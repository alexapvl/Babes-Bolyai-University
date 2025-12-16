package dsm;

import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.locks.ReentrantLock;
import java.util.function.Consumer;

/**
 * The core Distributed Shared Memory library.
 * 
 * This class implements the DSM mechanism that allows multiple processes
 * to share variables with consistency guarantees. Key features:
 * 
 * 1. WRITE OPERATIONS: Any subscriber can write to a variable. The write
 * is forwarded to the variable's owner, which serializes all writes
 * and broadcasts updates to all subscribers.
 * 
 * 2. TOTAL ORDERING: All subscribers receive updates in the same order.
 * This is achieved through sequence numbers assigned by the owner.
 * Pending updates are buffered and applied in order.
 * 
 * 3. COMPARE-AND-EXCHANGE (CAS): Atomic operation that only updates
 * a variable if its current value matches an expected value.
 * 
 * 4. CALLBACKS: Subscribers register callbacks to be notified of changes.
 * Callbacks are invoked in total order across all nodes.
 * 
 * Architecture:
 * - No central server; each variable has an owner that acts as sequencer
 * - Messages only flow between subscribers of a variable
 * - Network communication is abstracted through a message sending interface
 * 
 * Thread Safety:
 * - All public methods are thread-safe
 * - Internal state protected by concurrent data structures and locks
 */
public class DSMLibrary {
  private final int nodeId;
  private final int totalNodes;

  // Variables managed by this DSM instance
  private final ConcurrentHashMap<String, DSMVariable> variables;

  // Callbacks registered for variable changes
  private final ConcurrentHashMap<String, List<DSMCallback>> callbacks;

  // Pending updates waiting for earlier sequence numbers
  private final ConcurrentHashMap<String, PriorityQueue<PendingUpdate>> pendingUpdates;

  // Lock for applying updates in order
  private final ConcurrentHashMap<String, ReentrantLock> updateLocks;

  // Message sending interface (to be set by the node)
  private Consumer<MessageEnvelope> messageSender;

  // Pending responses for synchronous operations
  private final ConcurrentHashMap<String, CompletableFuture<Message>> pendingResponses;

  // For tracking acknowledgments
  private final ConcurrentHashMap<Long, Set<Integer>> pendingAcks;

  /**
   * Creates a new DSM library instance for a specific node.
   * 
   * @param nodeId     The unique identifier for this node (0 to totalNodes-1)
   * @param totalNodes The total number of nodes in the system
   */
  public DSMLibrary(int nodeId, int totalNodes) {
    this.nodeId = nodeId;
    this.totalNodes = totalNodes;
    this.variables = new ConcurrentHashMap<>();
    this.callbacks = new ConcurrentHashMap<>();
    this.pendingUpdates = new ConcurrentHashMap<>();
    this.updateLocks = new ConcurrentHashMap<>();
    this.pendingResponses = new ConcurrentHashMap<>();
    this.pendingAcks = new ConcurrentHashMap<>();
  }

  /**
   * Sets the message sender interface.
   * This must be called before any DSM operations.
   */
  public void setMessageSender(Consumer<MessageEnvelope> sender) {
    this.messageSender = sender;
  }

  // ============== Variable Management ==============

  /**
   * Registers a variable that this node owns.
   * The owner is responsible for serializing writes and assigning sequence
   * numbers.
   * 
   * @param name         The variable name
   * @param initialValue The initial value
   */
  public void registerOwnedVariable(String name, int initialValue) {
    registerOwnedVariable(name, initialValue, Set.of());
  }

  /**
   * Registers a variable that this node owns with known subscribers.
   * 
   * @param name             The variable name
   * @param initialValue     The initial value
   * @param otherSubscribers Other nodes that will subscribe to this variable
   */
  public void registerOwnedVariable(String name, int initialValue, Set<Integer> otherSubscribers) {
    DSMVariable var = new DSMVariable(name, initialValue, nodeId);
    for (int sub : otherSubscribers) {
      var.addSubscriber(sub);
    }
    variables.put(name, var);
    pendingUpdates.put(name, new PriorityQueue<>(Comparator.comparingLong(p -> p.seqNum)));
    updateLocks.put(name, new ReentrantLock());
    log("Registered owned variable: " + name + " = " + initialValue + " (subscribers: " + var.getSubscribers() + ")");
  }

  /**
   * Subscribes to a variable owned by another node.
   * Only subscribers can read and write to a variable.
   * 
   * @param name             The variable name
   * @param ownerId          The node ID that owns this variable
   * @param initialValue     The initial value (should match owner's value)
   * @param otherSubscribers Set of other subscriber node IDs
   */
  public void subscribeToVariable(String name, int ownerId, int initialValue, Set<Integer> otherSubscribers) {
    DSMVariable var = new DSMVariable(name, initialValue, ownerId);
    var.addSubscriber(nodeId);
    for (int sub : otherSubscribers) {
      var.addSubscriber(sub);
    }
    variables.put(name, var);
    pendingUpdates.put(name, new PriorityQueue<>(Comparator.comparingLong(p -> p.seqNum)));
    updateLocks.put(name, new ReentrantLock());
    log("Subscribed to variable: " + name + " (owner: " + ownerId + ")");
  }

  /**
   * Registers a callback for when a variable changes.
   * The callback will be invoked whenever the variable is updated,
   * whether by local or remote operations.
   * 
   * @param variableName The variable to watch
   * @param callback     The callback to invoke on changes
   */
  public void registerCallback(String variableName, DSMCallback callback) {
    callbacks.computeIfAbsent(variableName, k -> new CopyOnWriteArrayList<>()).add(callback);
    log("Registered callback for variable: " + variableName);
  }

  // ============== Read Operations ==============

  /**
   * Reads the current local value of a variable.
   * Note: This returns the last known value; updates may still be in transit.
   * 
   * @param variableName The variable to read
   * @return The current value
   * @throws IllegalArgumentException if the variable is not subscribed
   */
  public int read(String variableName) {
    DSMVariable var = variables.get(variableName);
    if (var == null) {
      throw new IllegalArgumentException("Variable not found: " + variableName);
    }
    return var.getValue();
  }

  // ============== Write Operations ==============

  /**
   * Writes a value to a variable.
   * 
   * If this node is the owner, the write is processed locally and
   * broadcast to all subscribers. If another node is the owner,
   * the write request is forwarded to the owner.
   * 
   * This operation is asynchronous - it returns after sending the request.
   * Use writeSync() for synchronous writes.
   * 
   * @param variableName The variable to write
   * @param value        The new value
   * @throws IllegalArgumentException if not subscribed to the variable
   */
  public void write(String variableName, int value) {
    DSMVariable var = variables.get(variableName);
    if (var == null) {
      throw new IllegalArgumentException("Variable not found: " + variableName);
    }
    if (!var.isSubscriber(nodeId)) {
      throw new IllegalArgumentException("Not subscribed to variable: " + variableName);
    }

    if (var.getOwnerId() == nodeId) {
      // We are the owner - process locally
      processWriteAsOwner(variableName, value, nodeId);
    } else {
      // Forward to owner
      Message msg = Message.writeRequest(variableName, value, nodeId, var.getOwnerId());
      sendMessage(msg, var.getOwnerId());
      log("Sent write request for " + variableName + " = " + value + " to owner " + var.getOwnerId());
    }
  }

  /**
   * Writes a value to a variable and waits for confirmation.
   * 
   * @param variableName The variable to write
   * @param value        The new value
   * @param timeoutMs    Timeout in milliseconds
   * @return true if the write was confirmed, false on timeout
   */
  public boolean writeSync(String variableName, int value, long timeoutMs) {
    DSMVariable var = variables.get(variableName);
    if (var == null) {
      throw new IllegalArgumentException("Variable not found: " + variableName);
    }

    String requestKey = variableName + "_write_" + System.nanoTime();
    CompletableFuture<Message> future = new CompletableFuture<>();
    pendingResponses.put(requestKey, future);

    write(variableName, value);

    try {
      future.get(timeoutMs, TimeUnit.MILLISECONDS);
      return true;
    } catch (Exception e) {
      return false;
    } finally {
      pendingResponses.remove(requestKey);
    }
  }

  // ============== Compare-And-Exchange ==============

  /**
   * Atomic compare-and-exchange operation.
   * 
   * Compares the variable's current value with expectedValue. If equal,
   * sets the variable to newValue and returns true. Otherwise, returns false.
   * 
   * This operation maintains total ordering with regular writes.
   * 
   * @param variableName  The variable to update
   * @param expectedValue The expected current value
   * @param newValue      The new value to set if comparison succeeds
   * @param timeoutMs     Timeout in milliseconds
   * @return true if the exchange was successful, false otherwise
   */
  public boolean compareAndExchange(String variableName, int expectedValue, int newValue, long timeoutMs) {
    DSMVariable var = variables.get(variableName);
    if (var == null) {
      throw new IllegalArgumentException("Variable not found: " + variableName);
    }
    if (!var.isSubscriber(nodeId)) {
      throw new IllegalArgumentException("Not subscribed to variable: " + variableName);
    }

    String requestKey = variableName + "_cas_" + System.nanoTime();
    CompletableFuture<Message> future = new CompletableFuture<>();
    pendingResponses.put(requestKey, future);

    if (var.getOwnerId() == nodeId) {
      // We are the owner - process locally
      boolean success = processCasAsOwner(variableName, expectedValue, newValue, nodeId);
      Message response = Message.casResponse(variableName, success, nodeId, nodeId);
      future.complete(response);
    } else {
      // Forward to owner
      Message msg = Message.casRequest(variableName, expectedValue, newValue, nodeId, var.getOwnerId());
      sendMessage(msg, var.getOwnerId());
      log("Sent CAS request for " + variableName + " (expect=" + expectedValue + ", new=" + newValue + ")");
    }

    try {
      Message response = future.get(timeoutMs, TimeUnit.MILLISECONDS);
      return response.isSuccess();
    } catch (Exception e) {
      log("CAS operation timed out for " + variableName);
      return false;
    } finally {
      pendingResponses.remove(requestKey);
    }
  }

  // ============== Message Handling ==============

  /**
   * Handles an incoming message from another node.
   * This method is called by the network layer when a message arrives.
   * 
   * @param message The received message
   */
  public void handleMessage(Message message) {
    log("Received: " + message);

    switch (message.getType()) {
      case WRITE_REQUEST:
        handleWriteRequest(message);
        break;
      case UPDATE_NOTIFICATION:
        handleUpdateNotification(message);
        break;
      case UPDATE_ACK:
        handleUpdateAck(message);
        break;
      case CAS_REQUEST:
        handleCasRequest(message);
        break;
      case CAS_RESPONSE:
        handleCasResponse(message);
        break;
      default:
        log("Unknown message type: " + message.getType());
    }
  }

  private void handleWriteRequest(Message message) {
    String varName = message.getVariableName();
    DSMVariable var = variables.get(varName);

    if (var == null || var.getOwnerId() != nodeId) {
      log("Received write request but not owner of " + varName);
      return;
    }

    processWriteAsOwner(varName, message.getValue(), message.getSenderId());
  }

  private void handleUpdateNotification(Message message) {
    String varName = message.getVariableName();
    long seqNum = message.getSequenceNumber();
    int newValue = message.getValue();

    ReentrantLock lock = updateLocks.get(varName);
    if (lock == null) {
      log("Received update for unknown variable: " + varName);
      return;
    }

    lock.lock();
    try {
      DSMVariable var = variables.get(varName);
      PriorityQueue<PendingUpdate> pending = pendingUpdates.get(varName);

      // Add to pending updates
      pending.add(new PendingUpdate(seqNum, newValue));

      // Apply all consecutive updates
      while (!pending.isEmpty() && pending.peek().seqNum == var.getLastAppliedSeqNum() + 1) {
        PendingUpdate update = pending.poll();
        int oldValue = var.setValue(update.value);
        var.setLastAppliedSeqNum(update.seqNum);

        // Invoke callbacks
        invokeCallbacks(varName, oldValue, update.value);

        log("Applied update: " + varName + " = " + update.value + " (seq=" + update.seqNum + ")");
      }

      // Send acknowledgment to owner
      Message ack = Message.updateAck(varName, seqNum, nodeId, var.getOwnerId());
      sendMessage(ack, var.getOwnerId());

    } finally {
      lock.unlock();
    }
  }

  private void handleUpdateAck(Message message) {
    long seqNum = message.getSequenceNumber();
    Set<Integer> acks = pendingAcks.get(seqNum);
    if (acks != null) {
      acks.add(message.getSenderId());
      // Could add logic here to track when all acks received
    }
  }

  private void handleCasRequest(Message message) {
    String varName = message.getVariableName();
    DSMVariable var = variables.get(varName);

    if (var == null || var.getOwnerId() != nodeId) {
      log("Received CAS request but not owner of " + varName);
      return;
    }

    boolean success = processCasAsOwner(varName, message.getExpectedValue(),
        message.getNewValue(), message.getSenderId());

    // Send response back to requester
    Message response = Message.casResponse(varName, success, nodeId, message.getSenderId());
    sendMessage(response, message.getSenderId());
  }

  private void handleCasResponse(Message message) {
    // Complete any pending futures waiting for CAS response
    String keyPrefix = message.getVariableName() + "_cas_";
    for (Map.Entry<String, CompletableFuture<Message>> entry : pendingResponses.entrySet()) {
      if (entry.getKey().startsWith(keyPrefix)) {
        entry.getValue().complete(message);
        break;
      }
    }
  }

  // ============== Owner Processing ==============

  /**
   * Processes a write as the variable owner.
   * Assigns a sequence number and broadcasts to all subscribers.
   */
  private void processWriteAsOwner(String varName, int newValue, int requesterId) {
    ReentrantLock lock = updateLocks.get(varName);
    lock.lock();
    try {
      DSMVariable var = variables.get(varName);
      long seqNum = var.getNextSequenceNumber();

      // Apply locally first
      int oldValue = var.setValue(newValue);
      var.setLastAppliedSeqNum(seqNum);

      // Invoke local callbacks
      invokeCallbacks(varName, oldValue, newValue);

      log("Owner processed write: " + varName + " = " + newValue + " (seq=" + seqNum + ")");

      // Track acks for this update
      Set<Integer> acks = ConcurrentHashMap.newKeySet();
      acks.add(nodeId); // Self-ack
      pendingAcks.put(seqNum, acks);

      // Broadcast to all other subscribers
      Message notification = Message.updateNotification(varName, newValue, seqNum, nodeId);
      for (int subscriberId : var.getSubscribers()) {
        if (subscriberId != nodeId) {
          sendMessage(notification, subscriberId);
        }
      }
    } finally {
      lock.unlock();
    }
  }

  /**
   * Processes a CAS operation as the variable owner.
   * If successful, broadcasts the update to all subscribers.
   */
  private boolean processCasAsOwner(String varName, int expectedValue, int newValue, int requesterId) {
    ReentrantLock lock = updateLocks.get(varName);
    lock.lock();
    try {
      DSMVariable var = variables.get(varName);

      // Check if current value matches expected
      if (var.getValue() != expectedValue) {
        log("CAS failed: " + varName + " current=" + var.getValue() + ", expected=" + expectedValue);
        return false;
      }

      // Perform the exchange using the same path as regular write
      // This ensures CAS maintains total ordering with writes
      long seqNum = var.getNextSequenceNumber();
      int oldValue = var.setValue(newValue);
      var.setLastAppliedSeqNum(seqNum);

      // Invoke local callbacks
      invokeCallbacks(varName, oldValue, newValue);

      log("CAS succeeded: " + varName + " = " + newValue + " (seq=" + seqNum + ")");

      // Broadcast to all other subscribers
      Message notification = Message.updateNotification(varName, newValue, seqNum, nodeId);
      for (int subscriberId : var.getSubscribers()) {
        if (subscriberId != nodeId) {
          sendMessage(notification, subscriberId);
        }
      }

      return true;
    } finally {
      lock.unlock();
    }
  }

  // ============== Helper Methods ==============

  private void invokeCallbacks(String varName, int oldValue, int newValue) {
    List<DSMCallback> varCallbacks = callbacks.get(varName);
    if (varCallbacks != null) {
      for (DSMCallback cb : varCallbacks) {
        try {
          cb.onVariableChanged(varName, oldValue, newValue);
        } catch (Exception e) {
          log("Callback error: " + e.getMessage());
        }
      }
    }
  }

  private void sendMessage(Message message, int targetNodeId) {
    if (messageSender != null) {
      messageSender.accept(new MessageEnvelope(message, targetNodeId));
    } else {
      log("Warning: No message sender configured!");
    }
  }

  private void log(String message) {
    System.out.println("[Node " + nodeId + "] " + message);
  }

  /**
   * Gets the node ID of this DSM instance.
   */
  public int getNodeId() {
    return nodeId;
  }

  /**
   * Gets a read-only view of all variables.
   */
  public Map<String, DSMVariable> getVariables() {
    return Collections.unmodifiableMap(variables);
  }

  // ============== Inner Classes ==============

  /**
   * Represents a pending update waiting to be applied.
   */
  private static class PendingUpdate {
    final long seqNum;
    final int value;

    PendingUpdate(long seqNum, int value) {
      this.seqNum = seqNum;
      this.value = value;
    }
  }

  /**
   * Envelope containing a message and its target node.
   */
  public static class MessageEnvelope {
    public final Message message;
    public final int targetNodeId;

    public MessageEnvelope(Message message, int targetNodeId) {
      this.message = message;
      this.targetNodeId = targetNodeId;
    }
  }
}
