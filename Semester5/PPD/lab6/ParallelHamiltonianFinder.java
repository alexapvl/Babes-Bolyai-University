import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.atomic.AtomicReference;

/**
 * Parallel implementation of Hamiltonian cycle finder using manual thread
 * management.
 * 
 * The algorithm distributes threads across branches at each level:
 * - If we have T threads and B branches at a node, each branch gets
 * approximately T/B threads
 * - Extra threads are distributed to the first branches (e.g., 8 threads, 3
 * branches → 3, 3, 2)
 * - When a branch has only 1 thread assigned, it continues with sequential
 * backtracking
 * 
 * Synchronization:
 * - Uses AtomicReference to safely share the found result across all threads
 * - Once a solution is found, all threads can detect this and stop early
 * - Uses ExecutorService to avoid thread creation overhead
 */
public class ParallelHamiltonianFinder implements HamiltonianCycleFinder {
    private final int numThreads;

    public ParallelHamiltonianFinder(int numThreads) {
        this.numThreads = numThreads;
    }

    @Override
    public HamiltonianResult findCycle(Graph graph, int startVertex) {
        long startTime = System.nanoTime();

        int n = graph.getVertexCount();

        // Shared state for the found result - thread-safe using AtomicReference
        AtomicReference<List<Integer>> foundCycle = new AtomicReference<>(null);

        // Create a fixed thread pool to avoid thread creation overhead
        ExecutorService executor = Executors.newFixedThreadPool(numThreads);

        try {
            // Initialize the search
            boolean[] visited = new boolean[n];
            List<Integer> path = new ArrayList<>();
            path.add(startVertex);
            visited[startVertex] = true;

            // Start parallel search
            parallelSearch(graph, startVertex, visited, path, n, numThreads, foundCycle, executor);
        } finally {
            executor.shutdown();
        }

        long endTime = System.nanoTime();

        List<Integer> result = foundCycle.get();
        if (result != null) {
            return HamiltonianResult.found(result, endTime - startTime);
        } else {
            return HamiltonianResult.notFound(endTime - startTime);
        }
    }

    /**
     * Performs the parallel search by distributing threads across neighbor
     * branches.
     */
    private void parallelSearch(Graph graph, int startVertex, boolean[] visited,
            List<Integer> path, int n, int availableThreads,
            AtomicReference<List<Integer>> foundCycle,
            ExecutorService executor) {
        // If solution already found, stop immediately
        if (foundCycle.get() != null) {
            return;
        }

        // Base case: if we've visited all vertices
        if (path.size() == n) {
            int lastVertex = path.get(path.size() - 1);
            if (graph.hasEdge(lastVertex, startVertex)) {
                List<Integer> cycle = new ArrayList<>(path);
                cycle.add(startVertex);
                foundCycle.compareAndSet(null, cycle);
            }
            return;
        }

        int currentVertex = path.get(path.size() - 1);
        List<Integer> neighbors = graph.getNeighbors(currentVertex);

        // Filter unvisited neighbors
        List<Integer> unvisitedNeighbors = new ArrayList<>();
        for (int neighbor : neighbors) {
            if (!visited[neighbor]) {
                unvisitedNeighbors.add(neighbor);
            }
        }

        if (unvisitedNeighbors.isEmpty()) {
            return;
        }

        // If only one thread or one neighbor, continue sequentially
        if (availableThreads <= 1 || unvisitedNeighbors.size() == 1) {
            sequentialSearch(graph, startVertex, visited, path, n, foundCycle);
            return;
        }

        // Distribute threads across branches
        int numBranches = unvisitedNeighbors.size();
        int[] threadsPerBranch = distributeThreads(availableThreads, numBranches);

        // Submit tasks to thread pool
        List<Future<?>> futures = new ArrayList<>();

        for (int i = 0; i < numBranches; i++) {
            final int branchThreads = threadsPerBranch[i];
            final int neighbor = unvisitedNeighbors.get(i);

            // Create copies of state for this branch
            final boolean[] branchVisited = visited.clone();
            final List<Integer> branchPath = new ArrayList<>(path);

            branchVisited[neighbor] = true;
            branchPath.add(neighbor);

            Future<?> future = executor.submit(() -> {
                parallelSearch(graph, startVertex, branchVisited, branchPath,
                        n, branchThreads, foundCycle, executor);
            });

            futures.add(future);
        }

        // Wait for all tasks to complete
        for (Future<?> future : futures) {
            try {
                future.get();
            } catch (Exception e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    /**
     * Sequential backtracking search - used when we have only one thread for a
     * subtree.
     */
    private void sequentialSearch(Graph graph, int startVertex, boolean[] visited,
            List<Integer> path, int n,
            AtomicReference<List<Integer>> foundCycle) {
        // Check if solution already found
        if (foundCycle.get() != null) {
            return;
        }

        // Base case
        if (path.size() == n) {
            int lastVertex = path.get(path.size() - 1);
            if (graph.hasEdge(lastVertex, startVertex)) {
                List<Integer> cycle = new ArrayList<>(path);
                cycle.add(startVertex);
                foundCycle.compareAndSet(null, cycle);
            }
            return;
        }

        int currentVertex = path.get(path.size() - 1);

        for (int neighbor : graph.getNeighbors(currentVertex)) {
            if (foundCycle.get() != null) {
                return;
            }

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                path.add(neighbor);

                sequentialSearch(graph, startVertex, visited, path, n, foundCycle);

                // Backtrack
                path.remove(path.size() - 1);
                visited[neighbor] = false;
            }
        }
    }

    /**
     * Distributes threads across branches as evenly as possible.
     * Extra threads go to the first branches.
     * 
     * Example: 8 threads, 3 branches → [3, 3, 2]
     * 7 threads, 4 branches → [2, 2, 2, 1]
     */
    private int[] distributeThreads(int totalThreads, int numBranches) {
        int[] distribution = new int[numBranches];
        int baseThreads = totalThreads / numBranches;
        int extraThreads = totalThreads % numBranches;

        for (int i = 0; i < numBranches; i++) {
            distribution[i] = baseThreads + (i < extraThreads ? 1 : 0);
        }

        return distribution;
    }

    @Override
    public String getName() {
        return "Parallel (" + numThreads + " threads)";
    }
}
