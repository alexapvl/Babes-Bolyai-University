import java.util.ArrayList;
import java.util.List;

/**
 * Sequential implementation of Hamiltonian cycle finder using backtracking.
 * This serves as the baseline for performance comparison.
 */
public class SequentialHamiltonianFinder implements HamiltonianCycleFinder {

    @Override
    public HamiltonianResult findCycle(Graph graph, int startVertex) {
        long startTime = System.nanoTime();

        int n = graph.getVertexCount();
        boolean[] visited = new boolean[n];
        List<Integer> path = new ArrayList<>();

        path.add(startVertex);
        visited[startVertex] = true;

        List<Integer> result = backtrack(graph, startVertex, visited, path, n);

        long endTime = System.nanoTime();

        if (result != null) {
            return HamiltonianResult.found(result, endTime - startTime);
        } else {
            return HamiltonianResult.notFound(endTime - startTime);
        }
    }

    /**
     * Recursive backtracking to find Hamiltonian cycle.
     *
     * @param graph       The graph to search
     * @param startVertex The original starting vertex (to check cycle completion)
     * @param visited     Array tracking visited vertices
     * @param path        Current path being explored
     * @param n           Total number of vertices
     * @return The Hamiltonian cycle if found, null otherwise
     */
    private List<Integer> backtrack(Graph graph, int startVertex, boolean[] visited,
            List<Integer> path, int n) {
        // Base case: if we've visited all vertices
        if (path.size() == n) {
            // Check if there's an edge back to the start vertex
            int lastVertex = path.get(path.size() - 1);
            if (graph.hasEdge(lastVertex, startVertex)) {
                // Found a Hamiltonian cycle
                List<Integer> cycle = new ArrayList<>(path);
                cycle.add(startVertex); // Complete the cycle
                return cycle;
            }
            return null;
        }

        int currentVertex = path.get(path.size() - 1);

        // Try all unvisited neighbors
        for (int neighbor : graph.getNeighbors(currentVertex)) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                path.add(neighbor);

                List<Integer> result = backtrack(graph, startVertex, visited, path, n);
                if (result != null) {
                    return result;
                }

                // Backtrack
                path.remove(path.size() - 1);
                visited[neighbor] = false;
            }
        }

        return null;
    }

    @Override
    public String getName() {
        return "Sequential Backtracking";
    }
}
