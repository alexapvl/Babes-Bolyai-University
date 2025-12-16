package dsm;

import java.util.Collections;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

/**
 * Represents a shared variable in the Distributed Shared Memory system.
 * 
 * Each DSMVariable has:
 * - A unique name (identifier)
 * - An integer value
 * - An owner node (responsible for serializing writes and total ordering)
 * - A set of subscriber node IDs
 * - A sequence number for ordering updates
 * 
 * The owner node acts as the "sequencer" for this variable:
 * - All write requests are forwarded to the owner
 * - The owner assigns sequence numbers and broadcasts updates
 * - This ensures total ordering of all updates to this variable
 * 
 * Thread Safety:
 * - Uses AtomicInteger for the value to handle concurrent local access
 * - Uses AtomicLong for sequence numbers
 * - Uses ConcurrentHashMap-based Set for subscribers
 */
public class DSMVariable {
  private final String name;
  private final AtomicInteger value;
  private final int ownerId;
  private final Set<Integer> subscribers;
  private final AtomicLong sequenceNumber;
  private volatile long lastAppliedSeqNum;

  /**
   * Creates a new DSM variable.
   * 
   * @param name         The unique identifier for this variable
   * @param initialValue The starting value
   * @param ownerId      The node ID that owns this variable (acts as sequencer)
   */
  public DSMVariable(String name, int initialValue, int ownerId) {
    this.name = name;
    this.value = new AtomicInteger(initialValue);
    this.ownerId = ownerId;
    this.subscribers = ConcurrentHashMap.newKeySet();
    this.sequenceNumber = new AtomicLong(0);
    this.lastAppliedSeqNum = 0;

    // Owner is automatically a subscriber
    this.subscribers.add(ownerId);
  }

  /**
   * Gets the variable's name.
   */
  public String getName() {
    return name;
  }

  /**
   * Gets the current value.
   * Note: In a distributed context, this may not reflect the most recent
   * value if updates are still propagating.
   */
  public int getValue() {
    return value.get();
  }

  /**
   * Sets the value directly (used internally when applying updates).
   * Returns the old value for callback purposes.
   */
  public int setValue(int newValue) {
    return value.getAndSet(newValue);
  }

  /**
   * Performs an atomic compare-and-exchange operation.
   * 
   * @param expectedValue The expected current value
   * @param newValue      The new value to set if current equals expected
   * @return true if the exchange was successful, false otherwise
   */
  public boolean compareAndExchange(int expectedValue, int newValue) {
    return value.compareAndSet(expectedValue, newValue);
  }

  /**
   * Gets the owner node ID.
   * The owner is responsible for serializing all writes to this variable.
   */
  public int getOwnerId() {
    return ownerId;
  }

  /**
   * Checks if a node is subscribed to this variable.
   */
  public boolean isSubscriber(int nodeId) {
    return subscribers.contains(nodeId);
  }

  /**
   * Adds a subscriber to this variable.
   */
  public void addSubscriber(int nodeId) {
    subscribers.add(nodeId);
  }

  /**
   * Gets all subscriber node IDs.
   */
  public Set<Integer> getSubscribers() {
    return Collections.unmodifiableSet(subscribers);
  }

  /**
   * Gets the next sequence number (used by owner when processing writes).
   */
  public long getNextSequenceNumber() {
    return sequenceNumber.incrementAndGet();
  }

  /**
   * Gets the last applied sequence number.
   */
  public long getLastAppliedSeqNum() {
    return lastAppliedSeqNum;
  }

  /**
   * Updates the last applied sequence number.
   */
  public void setLastAppliedSeqNum(long seqNum) {
    this.lastAppliedSeqNum = seqNum;
  }

  @Override
  public String toString() {
    return String.format("DSMVariable{name='%s', value=%d, owner=%d, subscribers=%s}",
        name, value.get(), ownerId, subscribers);
  }
}
