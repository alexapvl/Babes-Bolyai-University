import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Parallel implementation of Hamiltonian cycle finder using ForkJoinPool and
 * RecursiveTask.
 * 
 * The algorithm uses work-stealing from ForkJoinPool for automatic load
 * balancing:
 * - Each branch exploration is submitted as a RecursiveTask
 * - Tasks fork sub-tasks for each unvisited neighbor
 * - When the remaining work is small (threshold), it switches to sequential
 * execution
 * 
 * Synchronization:
 * - Uses AtomicBoolean to signal when a solution is found
 * - All tasks check this flag and return early if set
 * - The pool automatically handles work distribution and stealing
 */
public class ForkJoinHamiltonianFinder implements HamiltonianCycleFinder {
    private final int parallelism;
    private static final int SEQUENTIAL_THRESHOLD = 3; // Switch to sequential when path is this close to complete

    public ForkJoinHamiltonianFinder(int parallelism) {
        this.parallelism = parallelism;
    }

    @Override
    public HamiltonianResult findCycle(Graph graph, int startVertex) {
        long startTime = System.nanoTime();

        AtomicBoolean found = new AtomicBoolean(false);

        try (ForkJoinPool pool = new ForkJoinPool(parallelism)) {
            int n = graph.getVertexCount();
            boolean[] visited = new boolean[n];
            List<Integer> path = new ArrayList<>();
            path.add(startVertex);
            visited[startVertex] = true;

            HamiltonianTask rootTask = new HamiltonianTask(
                    graph, startVertex, visited, path, n, found);

            // Will block until the task is complete
            List<Integer> result = pool.invoke(rootTask);

            long endTime = System.nanoTime();

            if (result != null) {
                return HamiltonianResult.found(result, endTime - startTime);
            } else {
                return HamiltonianResult.notFound(endTime - startTime);
            }
        }
    }

    @Override
    public String getName() {
        return "ForkJoin (" + parallelism + " parallelism)";
    }

    /**
     * RecursiveTask that explores one branch of the search tree.
     * Returns the Hamiltonian cycle if found, null otherwise.
     */
    private static class HamiltonianTask extends RecursiveTask<List<Integer>> {
        private final Graph graph;
        private final int startVertex;
        private final boolean[] visited;
        private final List<Integer> path;
        private final int n;
        private final AtomicBoolean found;

        HamiltonianTask(Graph graph, int startVertex, boolean[] visited,
                List<Integer> path, int n, AtomicBoolean found) {
            this.graph = graph;
            this.startVertex = startVertex;
            this.visited = visited;
            this.path = path;
            this.n = n;
            this.found = found;
        }

        @Override
        protected List<Integer> compute() {
            // Check if another task already found the solution
            if (found.get()) {
                return null;
            }

            // Base case: visited all vertices
            if (path.size() == n) {
                int lastVertex = path.get(path.size() - 1);
                if (graph.hasEdge(lastVertex, startVertex)) {
                    List<Integer> cycle = new ArrayList<>(path);
                    cycle.add(startVertex);
                    found.set(true);
                    return cycle;
                }
                return null;
            }

            int currentVertex = path.get(path.size() - 1);
            List<Integer> neighbors = graph.getNeighbors(currentVertex);

            // Collect unvisited neighbors
            List<Integer> unvisitedNeighbors = new ArrayList<>();
            for (int neighbor : neighbors) {
                if (!visited[neighbor]) {
                    unvisitedNeighbors.add(neighbor);
                }
            }

            if (unvisitedNeighbors.isEmpty()) {
                return null;
            }

            // If close to the end or only one neighbor, use sequential search
            int remainingVertices = n - path.size();
            if (remainingVertices <= SEQUENTIAL_THRESHOLD || unvisitedNeighbors.size() == 1) {
                return sequentialSearch();
            }

            // Fork tasks for all neighbors except the last one
            List<HamiltonianTask> tasks = new ArrayList<>();

            for (int i = 0; i < unvisitedNeighbors.size() - 1; i++) {
                if (found.get()) {
                    break;
                }

                int neighbor = unvisitedNeighbors.get(i);

                // Create copies of state for this branch
                boolean[] branchVisited = visited.clone();
                List<Integer> branchPath = new ArrayList<>(path);

                branchVisited[neighbor] = true;
                branchPath.add(neighbor);

                HamiltonianTask task = new HamiltonianTask(
                        graph, startVertex, branchVisited, branchPath, n, found);
                task.fork();
                tasks.add(task);
            }

            // Compute the last branch directly (optimization: avoid fork overhead)
            List<Integer> result = null;
            if (!found.get() && !unvisitedNeighbors.isEmpty()) {
                int lastNeighbor = unvisitedNeighbors.get(unvisitedNeighbors.size() - 1);
                boolean[] lastVisited = visited.clone();
                List<Integer> lastPath = new ArrayList<>(path);

                lastVisited[lastNeighbor] = true;
                lastPath.add(lastNeighbor);

                HamiltonianTask lastTask = new HamiltonianTask(
                        graph, startVertex, lastVisited, lastPath, n, found);
                result = lastTask.compute();
            }

            // Join all forked tasks and collect results
            for (HamiltonianTask task : tasks) {
                if (result != null) {
                    task.cancel(true);
                } else {
                    List<Integer> taskResult = task.join();
                    if (taskResult != null) {
                        result = taskResult;
                    }
                }
            }

            return result;
        }

        /**
         * Sequential backtracking when we're close to the end or have limited
         * branching.
         */
        private List<Integer> sequentialSearch() {
            return sequentialBacktrack(visited.clone(), new ArrayList<>(path));
        }

        private List<Integer> sequentialBacktrack(boolean[] visited, List<Integer> path) {
            if (found.get()) {
                return null;
            }

            if (path.size() == n) {
                int lastVertex = path.get(path.size() - 1);
                if (graph.hasEdge(lastVertex, startVertex)) {
                    List<Integer> cycle = new ArrayList<>(path);
                    cycle.add(startVertex);
                    found.set(true);
                    return cycle;
                }
                return null;
            }

            int currentVertex = path.get(path.size() - 1);

            for (int neighbor : graph.getNeighbors(currentVertex)) {
                if (found.get()) {
                    return null;
                }

                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    path.add(neighbor);

                    List<Integer> result = sequentialBacktrack(visited, path);
                    if (result != null) {
                        return result;
                    }

                    path.remove(path.size() - 1);
                    visited[neighbor] = false;
                }
            }

            return null;
        }
    }
}
