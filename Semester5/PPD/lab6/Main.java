import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 * Main class for benchmarking Hamiltonian cycle finding algorithms.
 */
public class Main {

    public static void main(String[] args) {
        System.out.println("=".repeat(80));
        System.out.println("  HAMILTONIAN CYCLE FINDER - PARALLEL ALGORITHMS BENCHMARK");
        System.out.println("=".repeat(80));
        System.out.println();

        int[] threadCounts = { 2, 4, 8 };

        // Stress Test 1: Medium difficulty
        System.out.println("STRESS TEST 1: Medium Graph (18 vertices)");
        runStressTest(18, 54, 42, threadCounts);

        // Stress Test 2: Hard difficulty
        System.out.println("\nSTRESS TEST 2: Large Graph (20 vertices)");
        runStressTest(20, 80, 300, threadCounts);

        // Stress Test 3: Very Hard difficulty
        System.out.println("\nSTRESS TEST 3: Very Large Graph (22 vertices)");
        runStressTest(22, 100, 777, threadCounts);

        // Stress Test 4: No Hamiltonian cycle present
        System.out.println("\nSTRESS TEST 4: No Hamiltonian Cycle (20 vertices)");
        runNoCycleTest(20, 80, 999, threadCounts);
    }

    private static void runStressTest(int graphSize, int extraEdges, int seed, int[] threadCounts) {
        System.out.println("-".repeat(75));

        Random random = new Random(seed);
        Graph graph = Graph.createHardWithHamiltonianCycle(graphSize, extraEdges, random);

        System.out.println(graph.getStats());
        System.out.println();

        int startVertex = 0;

        List<HamiltonianCycleFinder> finders = new ArrayList<>();
        finders.add(new SequentialHamiltonianFinder());
        for (int threads : threadCounts) {
            finders.add(new ParallelHamiltonianFinder(threads));
            finders.add(new ForkJoinHamiltonianFinder(threads));
        }

        System.out.printf("%-35s | %15s | %10s | %5s | %s%n",
                "Algorithm", "Time (ms)", "Speedup", "Valid", "Cycle");
        System.out.println("-".repeat(110));

        double sequentialTime = 0;

        for (HamiltonianCycleFinder finder : finders) {
            // Warmup run
            finder.findCycle(graph, startVertex);

            // Timed run
            HamiltonianResult result = finder.findCycle(graph, startVertex);
            double time = result.getExecutionTimeMillis();

            String speedup;
            if (finder instanceof SequentialHamiltonianFinder) {
                sequentialTime = time;
                speedup = "1.00x";
            } else {
                if (time > 0 && sequentialTime > 0) {
                    speedup = String.format("%.2fx", sequentialTime / time);
                } else {
                    speedup = "N/A";
                }
            }

            String valid = result.isFound() && verifyCycle(graph, result.getCycle(), startVertex) ? "Yes" : "No";
            String cycleText = "Yes".equals(valid) ? result.getCycle().toString() : "-";

            System.out.printf("%-35s | %15.3f | %10s | %5s | %s%n",
                    finder.getName(), time, speedup, valid, cycleText);
        }

        System.out.println();
    }

    /**
     * Runs a test on a graph constructed to have no Hamiltonian cycle.
     * The graph is built as two strongly connected halves with one-way links from
     * the first half to the second, and no edges back. This guarantees the start
     * vertex cannot be part of a Hamiltonian cycle covering all vertices.
     */
    private static void runNoCycleTest(int graphSize, int extraEdges, int seed, int[] threadCounts) {
        System.out.println("-".repeat(75));

        Random random = new Random(seed);
        Graph graph = createNoHamiltonianGraph(graphSize, extraEdges, random);

        System.out.println(graph.getStats());
        System.out.println("Graph is constructed without a Hamiltonian cycle (one-way split components).");
        System.out.println();

        int startVertex = 0;

        List<HamiltonianCycleFinder> finders = new ArrayList<>();
        finders.add(new SequentialHamiltonianFinder());
        for (int threads : threadCounts) {
            finders.add(new ParallelHamiltonianFinder(threads));
            finders.add(new ForkJoinHamiltonianFinder(threads));
        }

        System.out.printf("%-35s | %15s | %10s | %5s | %s%n",
                "Algorithm", "Time (ms)", "Speedup", "Valid", "Cycle");
        System.out.println("-".repeat(110));

        double sequentialTime = 0;

        for (HamiltonianCycleFinder finder : finders) {
            // Warmup run
            finder.findCycle(graph, startVertex);

            // Timed run
            HamiltonianResult result = finder.findCycle(graph, startVertex);
            double time = result.getExecutionTimeMillis();

            String speedup;
            if (finder instanceof SequentialHamiltonianFinder) {
                sequentialTime = time;
                speedup = "1.00x";
            } else {
                if (time > 0 && sequentialTime > 0) {
                    speedup = String.format("%.2fx", sequentialTime / time);
                } else {
                    speedup = "N/A";
                }
            }

            String valid = result.isFound() && verifyCycle(graph, result.getCycle(), startVertex) ? "Yes" : "No";
            String cycleText = "Yes".equals(valid) ? result.getCycle().toString() : "-";

            System.out.printf("%-35s | %15.3f | %10s | %5s | %s%n",
                    finder.getName(), time, speedup, valid, cycleText);
        }

        System.out.println();
    }

    /**
     * Constructs a directed graph with no Hamiltonian cycle by splitting the
     * vertices into two strongly connected halves with one-way edges from the
     * first half to the second half and none back.
     */
    private static Graph createNoHamiltonianGraph(int vertexCount, int extraEdges, Random random) {
        Graph graph = new Graph(vertexCount);

        int half = vertexCount / 2;

        // Make each half strongly connected via a simple cycle
        for (int i = 0; i < half; i++) {
            graph.addEdge(i, (i + 1) % half);
        }
        for (int i = half; i < vertexCount; i++) {
            int next = i + 1 == vertexCount ? half : i + 1;
            graph.addEdge(i, next);
        }

        // Add one-way bridges from first half to second half
        for (int i = 0; i < half; i++) {
            int target = half + (i % (vertexCount - half));
            graph.addEdge(i, target);
        }

        // Add extra random edges within each half to increase branching, but keep
        // directionality split (no edges from second half back to first)
        int added = 0;
        while (added < extraEdges) {
            boolean inFirst = random.nextBoolean();
            if (inFirst) {
                int from = random.nextInt(half);
                int to = random.nextInt(half);
                if (from != to && !graph.hasEdge(from, to)) {
                    graph.addEdge(from, to);
                    added++;
                }
            } else {
                int from = half + random.nextInt(vertexCount - half);
                int to = half + random.nextInt(vertexCount - half);
                if (from != to && !graph.hasEdge(from, to)) {
                    graph.addEdge(from, to);
                    added++;
                }
            }
        }

        return graph;
    }

    /**
     * Verifies that a cycle is a valid Hamiltonian cycle.
     */
    private static boolean verifyCycle(Graph graph, List<Integer> cycle, int startVertex) {
        if (cycle == null || cycle.isEmpty()) {
            return false;
        }

        int n = graph.getVertexCount();

        // Check that cycle starts and ends at the start vertex
        if (cycle.get(0) != startVertex || cycle.get(cycle.size() - 1) != startVertex) {
            return false;
        }

        // Check that all vertices are visited exactly once (except start which appears
        // twice)
        if (cycle.size() != n + 1) {
            return false;
        }

        boolean[] visited = new boolean[n];
        for (int i = 0; i < cycle.size() - 1; i++) {
            int vertex = cycle.get(i);
            if (vertex < 0 || vertex >= n || visited[vertex]) {
                return false;
            }
            visited[vertex] = true;
        }

        // Check all edges exist
        for (int i = 0; i < cycle.size() - 1; i++) {
            if (!graph.hasEdge(cycle.get(i), cycle.get(i + 1))) {
                return false;
            }
        }

        return true;
    }
}
