package dsm;

import java.io.Serializable;

/**
 * Represents a message exchanged between DSM nodes.
 * 
 * Messages are the fundamental unit of communication in the DSM system.
 * Each message carries information about an operation on a shared variable,
 * along with metadata for ordering and routing.
 * 
 * Key fields:
 * - type: The operation being performed (WRITE, CAS, etc.)
 * - variableName: Which variable this message concerns
 * - value/expectedValue/newValue: Data for the operation
 * - sequenceNumber: For total ordering of updates
 * - senderId/targetId: Routing information
 */
public class Message implements Serializable {
  private static final long serialVersionUID = 1L;

  private final MessageType type;
  private final String variableName;
  private final int value;
  private final int expectedValue; // For CAS operations
  private final int newValue; // For CAS operations
  private final long sequenceNumber;
  private final int senderId;
  private final int targetId;
  private final boolean success; // For CAS response

  /**
   * Private constructor - use builder methods instead.
   */
  private Message(MessageType type, String variableName, int value,
      int expectedValue, int newValue, long sequenceNumber,
      int senderId, int targetId, boolean success) {
    this.type = type;
    this.variableName = variableName;
    this.value = value;
    this.expectedValue = expectedValue;
    this.newValue = newValue;
    this.sequenceNumber = sequenceNumber;
    this.senderId = senderId;
    this.targetId = targetId;
    this.success = success;
  }

  // ============== Factory Methods ==============

  /**
   * Creates a write request message.
   * Used when a subscriber wants to update a variable's value.
   */
  public static Message writeRequest(String varName, int value, int senderId, int targetId) {
    return new Message(MessageType.WRITE_REQUEST, varName, value, 0, 0, 0, senderId, targetId, false);
  }

  /**
   * Creates an update notification message.
   * Broadcast by the owner to inform subscribers of a new value.
   * The sequence number ensures total ordering.
   */
  public static Message updateNotification(String varName, int value, long seqNum, int senderId) {
    return new Message(MessageType.UPDATE_NOTIFICATION, varName, value, 0, 0, seqNum, senderId, -1, false);
  }

  /**
   * Creates an update acknowledgment message.
   */
  public static Message updateAck(String varName, long seqNum, int senderId, int targetId) {
    return new Message(MessageType.UPDATE_ACK, varName, 0, 0, 0, seqNum, senderId, targetId, false);
  }

  /**
   * Creates a compare-and-exchange request message.
   * The operation will succeed only if current value equals expectedValue.
   */
  public static Message casRequest(String varName, int expectedValue, int newValue, int senderId, int targetId) {
    return new Message(MessageType.CAS_REQUEST, varName, 0, expectedValue, newValue, 0, senderId, targetId, false);
  }

  /**
   * Creates a CAS response message indicating success or failure.
   */
  public static Message casResponse(String varName, boolean success, int senderId, int targetId) {
    return new Message(MessageType.CAS_RESPONSE, varName, 0, 0, 0, 0, senderId, targetId, success);
  }

  /**
   * Creates a subscription request message.
   */
  public static Message subscribe(String varName, int senderId, int targetId) {
    return new Message(MessageType.SUBSCRIBE, varName, 0, 0, 0, 0, senderId, targetId, false);
  }

  /**
   * Creates a subscription acknowledgment message.
   */
  public static Message subscribeAck(String varName, int currentValue, int senderId, int targetId) {
    return new Message(MessageType.SUBSCRIBE_ACK, varName, currentValue, 0, 0, 0, senderId, targetId, false);
  }

  // ============== Getters ==============

  public MessageType getType() {
    return type;
  }

  public String getVariableName() {
    return variableName;
  }

  public int getValue() {
    return value;
  }

  public int getExpectedValue() {
    return expectedValue;
  }

  public int getNewValue() {
    return newValue;
  }

  public long getSequenceNumber() {
    return sequenceNumber;
  }

  public int getSenderId() {
    return senderId;
  }

  public int getTargetId() {
    return targetId;
  }

  public boolean isSuccess() {
    return success;
  }

  @Override
  public String toString() {
    return String.format("Message{type=%s, var=%s, val=%d, seq=%d, from=%d, to=%d}",
        type, variableName, value, sequenceNumber, senderId, targetId);
  }
}
