import java.util.ArrayList;
import java.util.List;

/**
 * Performance benchmark for big number multiplication algorithms.
 */
public class BigNumberBenchmark {

    private static final int DEFAULT_NUM_THREADS = Runtime.getRuntime().availableProcessors();
    private static final int WARMUP_RUNS = 3;
    private static final int MEASUREMENT_RUNS = 5;

    public static class BenchmarkResult {
        String algorithmName;
        int numberDigits;
        long averageTimeNs;
        long minTimeNs;
        long maxTimeNs;
        boolean correctnessVerified;

        public BenchmarkResult(String algorithmName, int numberDigits,
                             long averageTimeNs, long minTimeNs, long maxTimeNs,
                             boolean correctnessVerified) {
            this.algorithmName = algorithmName;
            this.numberDigits = numberDigits;
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
            return String.format("%-30s | Digits: %6d | Avg: %10.2f ms | Min: %10.2f ms | Max: %10.2f ms | Correct: %s",
                    algorithmName, numberDigits,
                    getAverageTimeMs(), getMinTimeMs(), getMaxTimeMs(),
                    correctnessVerified ? "✓" : "✗");
        }
    }

    @FunctionalInterface
    interface AlgorithmFunction {
        BigNumber run();
    }

    public static List<BenchmarkResult> runBenchmark(int numDigits, int numThreads) {
        System.out.println("\n" + "=".repeat(120));
        System.out.println("Starting benchmark for big numbers with " + numDigits + " digits");
        System.out.println("Using " + numThreads + " threads for parallel variants");
        System.out.println("Warmup runs: " + WARMUP_RUNS + ", Measurement runs: " + MEASUREMENT_RUNS);
        System.out.println("=".repeat(120));

        List<BenchmarkResult> results = new ArrayList<>();

        // Generate test numbers
        BigNumber a = new BigNumber(numDigits);
        BigNumber b = new BigNumber(numDigits);

        System.out.println("\nGenerated numbers:");
        System.out.println("A: " + a.toCompactString());
        System.out.println("B: " + b.toCompactString());

        // Compute reference result
        System.out.println("\nComputing reference result...");
        BigNumber reference = BigNumberMultiplication.multiplySequential(a, b);
        System.out.println("Reference result: " + reference.toCompactString());

        // Benchmark 1: Regular Sequential
        System.out.println("\n[1/4] Benchmarking Regular Sequential...");
        BenchmarkResult result1 = benchmarkAlgorithm(
            "Regular Sequential",
            numDigits,
            () -> BigNumberMultiplication.multiplySequential(a, b),
            reference
        );
        results.add(result1);
        System.out.println(result1);

        // Benchmark 2: Regular Parallel
        System.out.println("\n[2/4] Benchmarking Regular Parallel...");
        BenchmarkResult result2 = benchmarkAlgorithm(
            "Regular Parallel",
            numDigits,
            () -> BigNumberMultiplication.multiplyParallel(a, b, numThreads),
            reference
        );
        results.add(result2);
        System.out.println(result2);

        // Benchmark 3: Karatsuba Sequential
        System.out.println("\n[3/4] Benchmarking Karatsuba Sequential...");
        BenchmarkResult result3 = benchmarkAlgorithm(
            "Karatsuba Sequential",
            numDigits,
            () -> BigNumberMultiplication.multiplyKaratsubaSequential(a, b),
            reference
        );
        results.add(result3);
        System.out.println(result3);

        // Benchmark 4: Karatsuba Parallel
        System.out.println("\n[4/4] Benchmarking Karatsuba Parallel...");
        BenchmarkResult result4 = benchmarkAlgorithm(
            "Karatsuba Parallel",
            numDigits,
            () -> BigNumberMultiplication.multiplyKaratsubaParallel(a, b, numThreads),
            reference
        );
        results.add(result4);
        System.out.println(result4);

        return results;
    }

    private static BenchmarkResult benchmarkAlgorithm(String name, int numDigits,
                                                     AlgorithmFunction algorithm,
                                                     BigNumber reference) {
        // Warmup
        for (int i = 0; i < WARMUP_RUNS; i++) {
            algorithm.run();
        }

        // Measurement
        long[] times = new long[MEASUREMENT_RUNS];
        BigNumber result = null;

        for (int i = 0; i < MEASUREMENT_RUNS; i++) {
            System.gc();
            try {
                Thread.sleep(100);
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

        return new BenchmarkResult(name, numDigits, average, min, max, correct);
    }

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

        if (results.isEmpty()) return;

        BenchmarkResult baseline = results.get(0);
        double baselineTime = baseline.averageTimeNs;

        for (int i = 1; i < results.size(); i++) {
            BenchmarkResult result = results.get(i);
            double speedup = baselineTime / result.averageTimeNs;
            System.out.printf("%-30s | Speedup: %.2fx%n", result.algorithmName, speedup);
        }

        System.out.println("=".repeat(120));
    }

    public static void runComprehensiveBenchmark() {
        int[] digitCounts = {100, 500, 1000, 2000, 5000};
        int numThreads = DEFAULT_NUM_THREADS;

        System.out.println("\n" + "█".repeat(120));
        System.out.println("█" + " ".repeat(118) + "█");
        System.out.println("█" + " ".repeat(38) + "COMPREHENSIVE BIG NUMBER MULTIPLICATION BENCHMARK" + " ".repeat(32) + "█");
        System.out.println("█" + " ".repeat(118) + "█");
        System.out.println("█".repeat(120));

        for (int digits : digitCounts) {
            List<BenchmarkResult> results = runBenchmark(digits, numThreads);
            printComparisonTable(results);

            System.out.println("\nContinuing to next benchmark...");
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        if (args.length > 0) {
            int digits = Integer.parseInt(args[0]);
            int numThreads = args.length > 1 ? Integer.parseInt(args[1]) : DEFAULT_NUM_THREADS;

            List<BenchmarkResult> results = runBenchmark(digits, numThreads);
            printComparisonTable(results);
        } else {
            runComprehensiveBenchmark();
        }
    }
}

