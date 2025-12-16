package dsm;

/**
 * Defines all message types used in the DSM communication protocol.
 * 
 * The DSM uses a message-passing approach where nodes communicate
 * through these different message types to coordinate variable updates
 * and maintain consistency.
 */
public enum MessageType {
  /**
   * Request to write a value to a variable.
   * Sent from any subscriber to the variable's owner node.
   */
  WRITE_REQUEST,

  /**
   * Notification that a variable has been updated.
   * Broadcast by the owner to all subscribers after processing a write.
   * Contains the sequence number for total ordering.
   */
  UPDATE_NOTIFICATION,

  /**
   * Acknowledgment that an update has been applied locally.
   * Sent from subscribers back to the owner.
   */
  UPDATE_ACK,

  /**
   * Compare-and-exchange request.
   * Atomically compares a variable with an expected value and,
   * if equal, sets it to a new value.
   */
  CAS_REQUEST,

  /**
   * Response to a CAS request indicating success or failure.
   */
  CAS_RESPONSE,

  /**
   * Subscription request to join a variable's subscriber list.
   */
  SUBSCRIBE,

  /**
   * Acknowledgment of subscription.
   */
  SUBSCRIBE_ACK
}
