import java.util.Collections;
import java.util.List;

/**
 * Represents the result of a Hamiltonian cycle search.
 */
public class HamiltonianResult {
  private final boolean found;
  private final List<Integer> cycle;
  private final long executionTimeNanos;

  private HamiltonianResult(boolean found, List<Integer> cycle, long executionTimeNanos) {
    this.found = found;
    this.cycle = cycle;
    this.executionTimeNanos = executionTimeNanos;
  }

  public static HamiltonianResult found(List<Integer> cycle, long executionTimeNanos) {
    return new HamiltonianResult(true, cycle, executionTimeNanos);
  }

  public static HamiltonianResult notFound(long executionTimeNanos) {
    return new HamiltonianResult(false, Collections.emptyList(), executionTimeNanos);
  }

  public boolean isFound() {
    return found;
  }

  public List<Integer> getCycle() {
    return Collections.unmodifiableList(cycle);
  }

  public long getExecutionTimeNanos() {
    return executionTimeNanos;
  }

  public double getExecutionTimeMillis() {
    return executionTimeNanos / 1_000_000.0;
  }

  @Override
  public String toString() {
    if (found) {
      return String.format("Hamiltonian cycle found: %s (%.2f ms)", cycle, getExecutionTimeMillis());
    } else {
      return String.format("No Hamiltonian cycle found (%.2f ms)", getExecutionTimeMillis());
    }
  }
}
