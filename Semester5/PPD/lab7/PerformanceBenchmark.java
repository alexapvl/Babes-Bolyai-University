import java.util.ArrayList;
import java.util.List;

/**
 * Performance benchmark for comparing different polynomial multiplication
 * algorithms.
 */
public class PerformanceBenchmark {

    private static final int DEFAULT_NUM_THREADS = Runtime.getRuntime().availableProcessors();
    private static final int WARMUP_RUNS = 3;
    private static final int MEASUREMENT_RUNS = 5;

    public static class BenchmarkResult {
        String algorithmName;
        int polynomialDegree;
        long averageTimeNs;
        long minTimeNs;
        long maxTimeNs;
        boolean correctnessVerified;

        public BenchmarkResult(String algorithmName, int polynomialDegree,
                long averageTimeNs, long minTimeNs, long maxTimeNs,
                boolean correctnessVerified) {
            this.algorithmName = algorithmName;
            this.polynomialDegree = polynomialDegree;
            this.averageTimeNs = averageTimeNs;
            this.minTimeNs = minTimeNs;
            this.maxTimeNs = maxTimeNs;
            this.correctnessVerified = correctnessVerified;
        }

        public double getAverageTimeMs() {
            return averageTimeNs / 1_000_000.0;
        }

        public double getMinTimeMs() {
            return minTimeNs / 1_000_000.0;
        }

        public double getMaxTimeMs() {
            return maxTimeNs / 1_000_000.0;
        }

        @Override
        public String toString() {
            return String.format("%-30s | Degree: %6d | Avg: %10.2f ms | Min: %10.2f ms | Max: %10.2f ms | Correct: %s",
                    algorithmName, polynomialDegree,
                    getAverageTimeMs(), getMinTimeMs(), getMaxTimeMs(),
                    correctnessVerified ? "✓" : "✗");
        }
    }

    /**
     * Benchmarks all four variants of polynomial multiplication.
     */
    public static List<BenchmarkResult> runBenchmark(int degree, int numThreads) {
        System.out.println("\n" + "=".repeat(120));
        System.out.println("Starting benchmark for polynomials of degree " + degree);
        System.out.println("Using " + numThreads + " threads for parallel variants");
        System.out.println("Warmup runs: " + WARMUP_RUNS + ", Measurement runs: " + MEASUREMENT_RUNS);
        System.out.println("=".repeat(120));

        List<BenchmarkResult> results = new ArrayList<>();

        // Generate test polynomials
        Polynomial p1 = new Polynomial(degree, 100);
        Polynomial p2 = new Polynomial(degree, 100);

        System.out.println("\nGenerated polynomials:");
        System.out.println("P1: " + p1.toCompactString());
        System.out.println("P2: " + p2.toCompactString());

        // Compute reference result (using sequential regular multiplication)
        System.out.println("\nComputing reference result...");
        Polynomial reference = RegularMultiplication.multiplySequential(p1, p2);
        System.out.println("Reference result: " + reference.toCompactString());

        // Benchmark 1: Regular Sequential
        System.out.println("\n[1/4] Benchmarking Regular Sequential...");
        BenchmarkResult result1 = benchmarkAlgorithm(
                "Regular Sequential",
                degree,
                () -> RegularMultiplication.multiplySequential(p1, p2),
                reference);
        results.add(result1);
        System.out.println(result1);

        // Benchmark 2.1: Regular Parallel (v1)
        // System.out.println("\n[2.1/4] Benchmarking Regular Parallel (v1)...");
        // BenchmarkResult result2_1 = benchmarkAlgorithm(
        // "Regular Parallel (v1)",
        // degree,
        // () -> RegularMultiplication.multiplyParallel(p1, p2, numThreads),
        // reference);
        // results.add(result2_1);
        // System.out.println(result2_1);

        // Benchmark 2.2: Regular Parallel (v2)
        System.out.println("\n[2.2/4] Benchmarking Regular Parallel (v2)...");
        BenchmarkResult result2 = benchmarkAlgorithm(
                "Regular Parallel (v2)",
                degree,
                () -> RegularMultiplication.multiplyParallelV2(p1, p2, numThreads),
                reference);
        results.add(result2);
        System.out.println(result2);

        // Benchmark 3: Karatsuba Sequential
        System.out.println("\n[3/4] Benchmarking Karatsuba Sequential...");
        BenchmarkResult result3 = benchmarkAlgorithm(
                "Karatsuba Sequential",
                degree,
                () -> KaratsubaMultiplication.multiplySequential(p1, p2),
                reference);
        results.add(result3);
        System.out.println(result3);

        // Benchmark 4: Karatsuba Parallel
        System.out.println("\n[4/4] Benchmarking Karatsuba Parallel...");
        BenchmarkResult result4 = benchmarkAlgorithm(
                "Karatsuba Parallel",
                degree,
                () -> KaratsubaMultiplication.multiplyParallel(p1, p2, numThreads),
                reference);
        results.add(result4);
        System.out.println(result4);

        return results;
    }

    private static BenchmarkResult benchmarkAlgorithm(String name, int degree,
            AlgorithmFunction algorithm,
            Polynomial reference) {
        // Warmup
        for (int i = 0; i < WARMUP_RUNS; i++) {
            algorithm.run();
        }

        // Measurement
        long[] times = new long[MEASUREMENT_RUNS];
        Polynomial result = null;

        for (int i = 0; i < MEASUREMENT_RUNS; i++) {
            System.gc(); // Suggest garbage collection before measurement
            try {
                Thread.sleep(100); // Brief pause between runs
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            long startTime = System.nanoTime();
            result = algorithm.run();
            long endTime = System.nanoTime();
            times[i] = endTime - startTime;

            System.out.print(".");
        }
        System.out.println();

        // Compute statistics
        long sum = 0;
        long min = Long.MAX_VALUE;
        long max = Long.MIN_VALUE;

        for (long time : times) {
            sum += time;
            min = Math.min(min, time);
            max = Math.max(max, time);
        }

        long average = sum / MEASUREMENT_RUNS;

        // Verify correctness
        boolean correct = result != null && result.equals(reference);

        return new BenchmarkResult(name, degree, average, min, max, correct);
    }

    @FunctionalInterface
    interface AlgorithmFunction {
        Polynomial run();
    }

    /**
     * Prints a comparison table of benchmark results.
     */
    public static void printComparisonTable(List<BenchmarkResult> results) {
        System.out.println("\n" + "=".repeat(120));
        System.out.println("PERFORMANCE COMPARISON");
        System.out.println("=".repeat(120));
        System.out.println();

        for (BenchmarkResult result : results) {
            System.out.println(result);
        }

        System.out.println("\n" + "-".repeat(120));
        System.out.println("SPEEDUP ANALYSIS (relative to Regular Sequential)");
        System.out.println("-".repeat(120));

        if (results.isEmpty())
            return;

        BenchmarkResult baseline = results.get(0);
        double baselineTime = baseline.averageTimeNs;

        for (int i = 1; i < results.size(); i++) {
            BenchmarkResult result = results.get(i);
            double speedup = baselineTime / result.averageTimeNs;
            System.out.printf("%-30s | Speedup: %.2fx%n", result.algorithmName, speedup);
        }

        System.out.println("=".repeat(120));
    }

    /**
     * Runs comprehensive benchmarks with multiple polynomial sizes.
     */
    public static void runComprehensiveBenchmark() {
        int[] degrees = { 100, 500, 1000, 2000, 5000 };
        int numThreads = DEFAULT_NUM_THREADS;

        System.out.println("\n" + "█".repeat(120));
        System.out.println("█" + " ".repeat(118) + "█");
        System.out.println(
                "█" + " ".repeat(35) + "COMPREHENSIVE POLYNOMIAL MULTIPLICATION BENCHMARK" + " ".repeat(33) + "█");
        System.out.println("█" + " ".repeat(118) + "█");
        System.out.println("█".repeat(120));

        for (int degree : degrees) {
            List<BenchmarkResult> results = runBenchmark(degree, numThreads);
            printComparisonTable(results);

            System.out.println("\nPress Enter to continue to next benchmark...");
            try {
                // Small delay instead of waiting for input in automated scenarios
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        if (args.length > 0) {
            // Single benchmark with specified degree
            int degree = Integer.parseInt(args[0]);
            int numThreads = args.length > 1 ? Integer.parseInt(args[1]) : DEFAULT_NUM_THREADS;

            List<BenchmarkResult> results = runBenchmark(degree, numThreads);
            printComparisonTable(results);
        } else {
            // Comprehensive benchmark
            runComprehensiveBenchmark();
        }
    }
}
