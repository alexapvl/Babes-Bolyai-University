import java.util.*;

/**
 * Represents a directed graph using an adjacency list.
 */
public class Graph {
  private final int vertexCount;
  private final List<List<Integer>> adjacencyList;

  public Graph(int vertexCount) {
    this.vertexCount = vertexCount;
    this.adjacencyList = new ArrayList<>(vertexCount);
    for (int i = 0; i < vertexCount; i++) {
      adjacencyList.add(new ArrayList<>());
    }
  }

  /**
   * Adds a directed edge from source to destination.
   */
  public void addEdge(int source, int destination) {
    if (source < 0 || source >= vertexCount || destination < 0 || destination >= vertexCount) {
      throw new IllegalArgumentException("Invalid vertex index");
    }
    if (!adjacencyList.get(source).contains(destination)) {
      adjacencyList.get(source).add(destination);
    }
  }

  /**
   * Returns the neighbors (out-edges) of a vertex.
   */
  public List<Integer> getNeighbors(int vertex) {
    return Collections.unmodifiableList(adjacencyList.get(vertex));
  }

  /**
   * Returns the out-degree of a vertex.
   */
  public int getOutDegree(int vertex) {
    return adjacencyList.get(vertex).size();
  }

  public int getVertexCount() {
    return vertexCount;
  }

  /**
   * Checks if there's an edge from source to destination.
   */
  public boolean hasEdge(int source, int destination) {
    return adjacencyList.get(source).contains(destination);
  }

  /**
   * Creates a random directed graph with guaranteed Hamiltonian cycle.
   * The graph is designed to be "easy" - cycle edges are found quickly.
   */
  public static Graph createWithHamiltonianCycle(int vertexCount, int extraEdges, Random random) {
    Graph graph = new Graph(vertexCount);

    // Create a random permutation to form a Hamiltonian cycle
    List<Integer> cycle = new ArrayList<>();
    for (int i = 0; i < vertexCount; i++) {
      cycle.add(i);
    }
    Collections.shuffle(cycle, random);

    // Add edges forming the Hamiltonian cycle
    for (int i = 0; i < vertexCount; i++) {
      int from = cycle.get(i);
      int to = cycle.get((i + 1) % vertexCount);
      graph.addEdge(from, to);
    }

    // Add extra random edges
    int added = 0;
    while (added < extraEdges) {
      int from = random.nextInt(vertexCount);
      int to = random.nextInt(vertexCount);
      if (from != to && !graph.hasEdge(from, to)) {
        graph.addEdge(from, to);
        added++;
      }
    }

    return graph;
  }

  /**
   * Creates a "hard" directed graph with guaranteed Hamiltonian cycle.
   * The graph is designed so that the backtracking algorithm needs to explore
   * many dead-end branches before finding the solution.
   * 
   * Strategy:
   * 1. Add many "decoy" edges first (dead-end paths)
   * 2. Add Hamiltonian cycle edges last
   * 3. Shuffle each adjacency list so cycle edges are explored later
   */
  public static Graph createHardWithHamiltonianCycle(int vertexCount, int extraEdges, Random random) {
    Graph graph = new Graph(vertexCount);

    // Create a random permutation to form a Hamiltonian cycle
    List<Integer> cycle = new ArrayList<>();
    for (int i = 0; i < vertexCount; i++) {
      cycle.add(i);
    }
    Collections.shuffle(cycle, random);

    // First, add many "decoy" edges (these will be explored first due to list
    // order)
    int added = 0;
    Set<Long> cycleEdges = new HashSet<>();
    for (int i = 0; i < vertexCount; i++) {
      int from = cycle.get(i);
      int to = cycle.get((i + 1) % vertexCount);
      cycleEdges.add((long) from * vertexCount + to);
    }

    // Add decoy edges
    while (added < extraEdges) {
      int from = random.nextInt(vertexCount);
      int to = random.nextInt(vertexCount);
      long edgeId = (long) from * vertexCount + to;
      if (from != to && !graph.hasEdge(from, to) && !cycleEdges.contains(edgeId)) {
        graph.addEdge(from, to);
        added++;
      }
    }

    // Now add edges forming the Hamiltonian cycle (added last, explored later)
    for (int i = 0; i < vertexCount; i++) {
      int from = cycle.get(i);
      int to = cycle.get((i + 1) % vertexCount);
      graph.addEdge(from, to);
    }

    // Shuffle all adjacency lists to make the cycle edges appear at random
    // positions
    // but with a bias toward the end (making them harder to find)
    for (int i = 0; i < vertexCount; i++) {
      List<Integer> neighbors = graph.adjacencyList.get(i);
      if (neighbors.size() > 1) {
        // Find the cycle edge for this vertex (if any)
        int cycleIdx = cycle.indexOf(i);
        int nextInCycle = cycle.get((cycleIdx + 1) % vertexCount);

        // Remove the cycle edge if present
        int cycleEdgeIdx = neighbors.indexOf(nextInCycle);
        if (cycleEdgeIdx >= 0) {
          neighbors.remove(cycleEdgeIdx);
          // Shuffle the rest
          Collections.shuffle(neighbors, random);
          // Add cycle edge back at the end (so it's explored last)
          neighbors.add(nextInCycle);
        } else {
          Collections.shuffle(neighbors, random);
        }
      }
    }

    return graph;
  }

  /**
   * Creates a random directed graph (may not have Hamiltonian cycle).
   */
  public static Graph createRandom(int vertexCount, double edgeProbability, Random random) {
    Graph graph = new Graph(vertexCount);

    for (int i = 0; i < vertexCount; i++) {
      for (int j = 0; j < vertexCount; j++) {
        if (i != j && random.nextDouble() < edgeProbability) {
          graph.addEdge(i, j);
        }
      }
    }

    return graph;
  }

  @Override
  public String toString() {
    StringBuilder sb = new StringBuilder();
    sb.append("Graph with ").append(vertexCount).append(" vertices:\n");
    for (int i = 0; i < vertexCount; i++) {
      sb.append(i).append(" -> ").append(adjacencyList.get(i)).append("\n");
    }
    return sb.toString();
  }

  /**
   * Returns statistics about the graph.
   */
  public String getStats() {
    int totalEdges = 0;
    int maxDegree = 0;
    int minDegree = Integer.MAX_VALUE;

    for (List<Integer> neighbors : adjacencyList) {
      int degree = neighbors.size();
      totalEdges += degree;
      maxDegree = Math.max(maxDegree, degree);
      minDegree = Math.min(minDegree, degree);
    }

    double avgDegree = (double) totalEdges / vertexCount;

    return String.format("Vertices: %d, Edges: %d, Avg degree: %.2f, Min: %d, Max: %d",
        vertexCount, totalEdges, avgDegree, minDegree, maxDegree);
  }
}
