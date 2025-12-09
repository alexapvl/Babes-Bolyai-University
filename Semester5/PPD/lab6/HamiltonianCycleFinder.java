/**
 * Interface for Hamiltonian cycle finding algorithms.
 */
public interface HamiltonianCycleFinder {
  /**
   * Finds a Hamiltonian cycle in the given graph starting from the specified
   * vertex.
   *
   * @param graph       The directed graph to search
   * @param startVertex The vertex to start the search from
   * @return HamiltonianResult containing the cycle if found
   */
  HamiltonianResult findCycle(Graph graph, int startVertex);

  /**
   * Returns the name of the algorithm implementation.
   */
  String getName();
}
