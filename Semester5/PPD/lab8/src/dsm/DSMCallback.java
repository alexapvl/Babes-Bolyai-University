package dsm;

/**
 * Callback interface for receiving notifications about variable changes.
 * 
 * When a shared variable is modified (either locally or by a remote node),
 * all subscribers to that variable will have their callback invoked.
 * 
 * IMPORTANT: The DSM guarantees that all processes will receive callbacks
 * in the same order. If variable A changes before variable B on one node,
 * all other nodes will also see A's change before B's change.
 * 
 * This is achieved through:
 * 1. Each variable has an "owner" node that serializes all writes
 * 2. The owner assigns sequence numbers to each update
 * 3. Subscribers apply updates in sequence number order
 * 
 * Usage example:
 * 
 * <pre>
 * dsm.subscribe("counter", (varName, oldValue, newValue) -> {
 *   System.out.println(varName + " changed from " + oldValue + " to " + newValue);
 * });
 * </pre>
 */
@FunctionalInterface
public interface DSMCallback {
  /**
   * Called when a subscribed variable's value changes.
   * 
   * @param variableName The name of the variable that changed
   * @param oldValue     The previous value of the variable
   * @param newValue     The new value of the variable
   */
  void onVariableChanged(String variableName, int oldValue, int newValue);
}
