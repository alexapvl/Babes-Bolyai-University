package dsm;

import java.util.concurrent.*;
import java.util.Map;
import java.util.Set;

/**
 * Represents a single node (process) in the Distributed Shared Memory system.
 * 
 * A DSMNode encapsulates:
 * - A DSMLibrary instance for managing shared variables
 * - A message queue for receiving messages from other nodes
 * - A background thread for processing incoming messages
 * 
 * In a real distributed system, nodes would communicate over the network.
 * This implementation simulates network communication using in-memory
 * message queues, which is suitable for demonstrating the DSM concepts.
 * 
 * Network Simulation:
 * - Messages are placed in the target node's queue
 * - An optional delay can simulate network latency
 * - Messages are processed asynchronously
 * 
 * Usage:
 * 1. Create nodes and connect them using setNodeNetwork()
 * 2. Register variables and subscribe to variables
 * 3. Start the node's message processing with start()
 * 4. Perform read/write/CAS operations
 * 5. Stop the node with stop()
 */
public class DSMNode {
  private final int nodeId;
  private final DSMLibrary dsm;
  private final BlockingQueue<Message> messageQueue;
  private final ExecutorService messageProcessor;
  private volatile boolean running;

  // Reference to all nodes in the network (for message routing)
  private Map<Integer, DSMNode> nodeNetwork;

  // Simulated network delay in milliseconds (0 for no delay)
  private final int networkDelayMs;

  /**
   * Creates a new DSM node.
   * 
   * @param nodeId     Unique identifier for this node (0 to totalNodes-1)
   * @param totalNodes Total number of nodes in the system
   */
  public DSMNode(int nodeId, int totalNodes) {
    this(nodeId, totalNodes, 0);
  }

  /**
   * Creates a new DSM node with simulated network delay.
   * 
   * @param nodeId         Unique identifier for this node
   * @param totalNodes     Total number of nodes in the system
   * @param networkDelayMs Simulated network delay in milliseconds
   */
  public DSMNode(int nodeId, int totalNodes, int networkDelayMs) {
    this.nodeId = nodeId;
    this.networkDelayMs = networkDelayMs;
    this.dsm = new DSMLibrary(nodeId, totalNodes);
    this.messageQueue = new LinkedBlockingQueue<>();
    this.messageProcessor = Executors.newSingleThreadExecutor(r -> {
      Thread t = new Thread(r, "DSMNode-" + nodeId + "-MessageProcessor");
      t.setDaemon(true);
      return t;
    });
    this.running = false;

    // Configure DSM to use our message routing
    dsm.setMessageSender(envelope -> sendMessageToNode(envelope.message, envelope.targetNodeId));
  }

  /**
   * Sets the node network for message routing.
   * Must be called before starting the node.
   * 
   * @param network Map of node ID to DSMNode instances
   */
  public void setNodeNetwork(Map<Integer, DSMNode> network) {
    this.nodeNetwork = network;
  }

  /**
   * Starts the message processing thread.
   */
  public void start() {
    if (running)
      return;
    running = true;

    messageProcessor.submit(() -> {
      while (running) {
        try {
          Message message = messageQueue.poll(100, TimeUnit.MILLISECONDS);
          if (message != null) {
            dsm.handleMessage(message);
          }
        } catch (InterruptedException e) {
          Thread.currentThread().interrupt();
          break;
        } catch (Exception e) {
          System.err.println("[Node " + nodeId + "] Error processing message: " + e.getMessage());
          e.printStackTrace();
        }
      }
    });

    System.out.println("[Node " + nodeId + "] Started");
  }

  /**
   * Stops the message processing thread.
   */
  public void stop() {
    running = false;
    messageProcessor.shutdownNow();
    System.out.println("[Node " + nodeId + "] Stopped");
  }

  /**
   * Receives a message from another node.
   * This method is called by the sender node.
   * 
   * @param message The message to receive
   */
  public void receiveMessage(Message message) {
    if (networkDelayMs > 0) {
      // Simulate network delay asynchronously
      CompletableFuture.runAsync(() -> {
        try {
          Thread.sleep(networkDelayMs);
          messageQueue.put(message);
        } catch (InterruptedException e) {
          Thread.currentThread().interrupt();
        }
      });
    } else {
      try {
        messageQueue.put(message);
      } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
      }
    }
  }

  /**
   * Sends a message to another node.
   */
  private void sendMessageToNode(Message message, int targetNodeId) {
    if (nodeNetwork == null) {
      throw new IllegalStateException("Node network not configured");
    }

    DSMNode targetNode = nodeNetwork.get(targetNodeId);
    if (targetNode == null) {
      throw new IllegalArgumentException("Unknown target node: " + targetNodeId);
    }

    targetNode.receiveMessage(message);
  }

  // ============== DSM Operations (delegated to DSMLibrary) ==============

  /**
   * Registers a variable that this node owns.
   * 
   * @param name         Variable name
   * @param initialValue Initial value
   */
  public void registerVariable(String name, int initialValue) {
    dsm.registerOwnedVariable(name, initialValue);
  }

  /**
   * Registers a variable that this node owns, with known subscribers.
   * 
   * @param name         Variable name
   * @param initialValue Initial value
   * @param subscribers  Other nodes that will subscribe to this variable
   */
  public void registerVariable(String name, int initialValue, Set<Integer> subscribers) {
    dsm.registerOwnedVariable(name, initialValue, subscribers);
  }

  /**
   * Subscribes to a variable owned by another node.
   * 
   * @param name             Variable name
   * @param ownerId          ID of the node that owns the variable
   * @param initialValue     Initial value
   * @param otherSubscribers Other nodes that subscribe to this variable
   */
  public void subscribe(String name, int ownerId, int initialValue, Set<Integer> otherSubscribers) {
    dsm.subscribeToVariable(name, ownerId, initialValue, otherSubscribers);
  }

  /**
   * Registers a callback for variable changes.
   * 
   * @param variableName Variable to watch
   * @param callback     Callback to invoke on changes
   */
  public void onVariableChange(String variableName, DSMCallback callback) {
    dsm.registerCallback(variableName, callback);
  }

  /**
   * Reads a variable's current value.
   * 
   * @param variableName Variable to read
   * @return The current value
   */
  public int read(String variableName) {
    return dsm.read(variableName);
  }

  /**
   * Writes a value to a variable (asynchronous).
   * 
   * @param variableName Variable to write
   * @param value        New value
   */
  public void write(String variableName, int value) {
    dsm.write(variableName, value);
  }

  /**
   * Writes a value to a variable and waits for confirmation.
   * 
   * @param variableName Variable to write
   * @param value        New value
   * @param timeoutMs    Timeout in milliseconds
   * @return true if confirmed, false on timeout
   */
  public boolean writeSync(String variableName, int value, long timeoutMs) {
    return dsm.writeSync(variableName, value, timeoutMs);
  }

  /**
   * Performs an atomic compare-and-exchange operation.
   * 
   * @param variableName  Variable to update
   * @param expectedValue Expected current value
   * @param newValue      New value if comparison succeeds
   * @param timeoutMs     Timeout in milliseconds
   * @return true if exchange succeeded, false otherwise
   */
  public boolean compareAndExchange(String variableName, int expectedValue, int newValue, long timeoutMs) {
    return dsm.compareAndExchange(variableName, expectedValue, newValue, timeoutMs);
  }

  // ============== Getters ==============

  public int getNodeId() {
    return nodeId;
  }

  public DSMLibrary getDsm() {
    return dsm;
  }

  public boolean isRunning() {
    return running;
  }
}
