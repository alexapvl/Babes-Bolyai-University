import mpi.*;
import java.util.Arrays;
import java.util.Random;

/**
 * Comprehensive benchmark comparing MPI distributed implementations
 * with sequential implementations for polynomial and big number multiplication.
 * 
 * This benchmark measures:
 * 1. Sequential Regular multiplication (baseline)
 * 2. Sequential Karatsuba multiplication
 * 3. MPI Regular multiplication
 * 4. MPI Karatsuba multiplication
 * 
 * And computes speedup ratios and efficiency metrics.
 */
public class MPIBenchmark {

    private static final int WARMUP_RUNS = 2;
    private static final int MEASUREMENT_RUNS = 5;
    private static final int KARATSUBA_THRESHOLD = 32;

    public static void main(String[] args) throws Exception {
        MPI.Init(args);

        int rank = MPI.COMM_WORLD.Rank();
        int size = MPI.COMM_WORLD.Size();

        if (rank == 0) {
            printHeader(size);
        }

        // Polynomial benchmarks
        if (rank == 0) {
            System.out.println("\n" + "█".repeat(80));
            System.out.println("                    POLYNOMIAL MULTIPLICATION BENCHMARKS");
            System.out.println("█".repeat(80));
        }

        int[] polyDegrees = { 100, 500, 1000, 2000, 5000, 10000 };
        for (int degree : polyDegrees) {
            runPolynomialBenchmark(rank, size, degree);
        }

        // Big number benchmarks
        if (rank == 0) {
            System.out.println("\n" + "█".repeat(80));
            System.out.println("                    BIG NUMBER MULTIPLICATION BENCHMARKS");
            System.out.println("█".repeat(80));
        }

        int[] numDigits = { 100, 500, 1000, 2000, 5000, 10000 };
        for (int digits : numDigits) {
            runBigNumberBenchmark(rank, size, digits);
        }

        // Summary
        if (rank == 0) {
            printSummary();
        }

        MPI.Finalize();
    }

    private static void printHeader(int numProcesses) {
        System.out.println();
        System.out.println("╔" + "═".repeat(78) + "╗");
        System.out.println("║" + " ".repeat(20) + "MPI DISTRIBUTED MULTIPLICATION BENCHMARK" + " ".repeat(18) + "║");
        System.out.println("║" + " ".repeat(30) + "LAB 7 - PPD" + " ".repeat(37) + "║");
        System.out.println("╠" + "═".repeat(78) + "╣");
        System.out.printf("║  Number of MPI Processes: %-50d║%n", numProcesses);
        System.out.printf("║  Warmup Runs: %-62d║%n", WARMUP_RUNS);
        System.out.printf("║  Measurement Runs: %-57d║%n", MEASUREMENT_RUNS);
        System.out.println("╚" + "═".repeat(78) + "╝");
    }

    private static void runPolynomialBenchmark(int rank, int size, int degree) throws Exception {
        MPI.COMM_WORLD.Barrier();

        int[] p1 = null;
        int[] p2 = null;

        if (rank == 0) {
            System.out.println("\n┌" + "─".repeat(78) + "┐");
            System.out.printf("│  Polynomial Degree: %-56d│%n", degree);
            System.out.println("├" + "─".repeat(78) + "┤");

            // Generate test polynomials
            Random rand = new Random(42 + degree);
            p1 = new int[degree + 1];
            p2 = new int[degree + 1];
            for (int i = 0; i <= degree; i++) {
                p1[i] = rand.nextInt(100);
                p2[i] = rand.nextInt(100);
            }
            p1[degree] = Math.max(1, p1[degree]);
            p2[degree] = Math.max(1, p2[degree]);
        }

        // Benchmark Sequential Regular
        double seqRegTime = 0;
        int[] seqResult = null;
        if (rank == 0) {
            seqRegTime = benchmarkSequentialRegular(p1, p2);
            seqResult = multiplySequential(p1, p2);
            System.out.printf("│  Sequential Regular:      %10.2f ms                                      │%n",
                    seqRegTime);
        }

        // Benchmark Sequential Karatsuba
        double seqKarTime = 0;
        if (rank == 0) {
            seqKarTime = benchmarkSequentialKaratsuba(p1, p2);
            System.out.printf("│  Sequential Karatsuba:    %10.2f ms                                      │%n",
                    seqKarTime);
        }

        // Benchmark MPI Regular
        MPI.COMM_WORLD.Barrier();
        double mpiRegTime = benchmarkMPIRegular(p1, p2, rank, size);
        if (rank == 0) {
            double speedup = seqRegTime / mpiRegTime;
            double efficiency = speedup / size * 100;
            System.out.printf("│  MPI Regular (%d proc):   %10.2f ms  (Speedup: %.2fx, Efficiency: %.1f%%)  │%n",
                    size, mpiRegTime, speedup, efficiency);
        }

        // Benchmark MPI Karatsuba
        MPI.COMM_WORLD.Barrier();
        double mpiKarTime = benchmarkMPIKaratsuba(p1, p2, rank, size);
        if (rank == 0) {
            double speedup = seqKarTime / mpiKarTime;
            double efficiency = speedup / size * 100;
            System.out.printf("│  MPI Karatsuba (%d proc): %10.2f ms  (Speedup: %.2fx, Efficiency: %.1f%%)  │%n",
                    size, mpiKarTime, speedup, efficiency);
            System.out.println("└" + "─".repeat(78) + "┘");
        }
    }

    private static void runBigNumberBenchmark(int rank, int size, int numDigits) throws Exception {
        MPI.COMM_WORLD.Barrier();

        int[] num1 = null;
        int[] num2 = null;

        if (rank == 0) {
            System.out.println("\n┌" + "─".repeat(78) + "┐");
            System.out.printf("│  Number of Digits: %-57d│%n", numDigits);
            System.out.println("├" + "─".repeat(78) + "┤");

            // Generate test numbers
            Random rand = new Random(42 + numDigits);
            num1 = new int[numDigits];
            num2 = new int[numDigits];
            for (int i = 0; i < numDigits - 1; i++) {
                num1[i] = rand.nextInt(10);
                num2[i] = rand.nextInt(10);
            }
            num1[numDigits - 1] = rand.nextInt(9) + 1;
            num2[numDigits - 1] = rand.nextInt(9) + 1;
        }

        // Benchmark Sequential Regular
        double seqRegTime = 0;
        if (rank == 0) {
            seqRegTime = benchmarkBigNumSequentialRegular(num1, num2);
            System.out.printf("│  Sequential Regular:      %10.2f ms                                      │%n",
                    seqRegTime);
        }

        // Benchmark Sequential Karatsuba
        double seqKarTime = 0;
        if (rank == 0) {
            seqKarTime = benchmarkBigNumSequentialKaratsuba(num1, num2);
            System.out.printf("│  Sequential Karatsuba:    %10.2f ms                                      │%n",
                    seqKarTime);
        }

        // Benchmark MPI Regular
        MPI.COMM_WORLD.Barrier();
        double mpiRegTime = benchmarkBigNumMPIRegular(num1, num2, rank, size);
        if (rank == 0) {
            double speedup = seqRegTime / mpiRegTime;
            double efficiency = speedup / size * 100;
            System.out.printf("│  MPI Regular (%d proc):   %10.2f ms  (Speedup: %.2fx, Efficiency: %.1f%%)  │%n",
                    size, mpiRegTime, speedup, efficiency);
        }

        // Benchmark MPI Karatsuba
        MPI.COMM_WORLD.Barrier();
        double mpiKarTime = benchmarkBigNumMPIKaratsuba(num1, num2, rank, size);
        if (rank == 0) {
            double speedup = seqKarTime / mpiKarTime;
            double efficiency = speedup / size * 100;
            System.out.printf("│  MPI Karatsuba (%d proc): %10.2f ms  (Speedup: %.2fx, Efficiency: %.1f%%)  │%n",
                    size, mpiKarTime, speedup, efficiency);
            System.out.println("└" + "─".repeat(78) + "┘");
        }
    }

    // ========================================================================
    // POLYNOMIAL BENCHMARK METHODS
    // ========================================================================

    private static double benchmarkSequentialRegular(int[] p1, int[] p2) {
        // Warmup
        for (int i = 0; i < WARMUP_RUNS; i++) {
            multiplySequential(p1, p2);
        }

        // Measure
        long totalTime = 0;
        for (int i = 0; i < MEASUREMENT_RUNS; i++) {
            long start = System.nanoTime();
            multiplySequential(p1, p2);
            totalTime += System.nanoTime() - start;
        }

        return (totalTime / MEASUREMENT_RUNS) / 1_000_000.0;
    }

    private static double benchmarkSequentialKaratsuba(int[] p1, int[] p2) {
        // Warmup
        for (int i = 0; i < WARMUP_RUNS; i++) {
            multiplyKaratsuba(p1, p2);
        }

        // Measure
        long totalTime = 0;
        for (int i = 0; i < MEASUREMENT_RUNS; i++) {
            long start = System.nanoTime();
            multiplyKaratsuba(p1, p2);
            totalTime += System.nanoTime() - start;
        }

        return (totalTime / MEASUREMENT_RUNS) / 1_000_000.0;
    }

    private static double benchmarkMPIRegular(int[] p1, int[] p2, int rank, int size)
            throws Exception {
        // Warmup
        for (int i = 0; i < WARMUP_RUNS; i++) {
            MPIPolynomial.multiplyRegularMPI(p1, p2, rank, size);
            MPI.COMM_WORLD.Barrier();
        }

        // Measure
        long totalTime = 0;
        for (int i = 0; i < MEASUREMENT_RUNS; i++) {
            MPI.COMM_WORLD.Barrier();
            long start = System.nanoTime();
            MPIPolynomial.multiplyRegularMPI(p1, p2, rank, size);
            MPI.COMM_WORLD.Barrier();
            totalTime += System.nanoTime() - start;
        }

        return (totalTime / MEASUREMENT_RUNS) / 1_000_000.0;
    }

    private static double benchmarkMPIKaratsuba(int[] p1, int[] p2, int rank, int size)
            throws Exception {
        // Warmup
        for (int i = 0; i < WARMUP_RUNS; i++) {
            MPIPolynomial.multiplyKaratsubaMPI(p1, p2, rank, size);
            MPI.COMM_WORLD.Barrier();
        }

        // Measure
        long totalTime = 0;
        for (int i = 0; i < MEASUREMENT_RUNS; i++) {
            MPI.COMM_WORLD.Barrier();
            long start = System.nanoTime();
            MPIPolynomial.multiplyKaratsubaMPI(p1, p2, rank, size);
            MPI.COMM_WORLD.Barrier();
            totalTime += System.nanoTime() - start;
        }

        return (totalTime / MEASUREMENT_RUNS) / 1_000_000.0;
    }

    // ========================================================================
    // BIG NUMBER BENCHMARK METHODS
    // ========================================================================

    private static double benchmarkBigNumSequentialRegular(int[] num1, int[] num2) {
        // Warmup
        for (int i = 0; i < WARMUP_RUNS; i++) {
            MPIBigNumber.multiplySequential(num1, num2);
        }

        // Measure
        long totalTime = 0;
        for (int i = 0; i < MEASUREMENT_RUNS; i++) {
            long start = System.nanoTime();
            MPIBigNumber.multiplySequential(num1, num2);
            totalTime += System.nanoTime() - start;
        }

        return (totalTime / MEASUREMENT_RUNS) / 1_000_000.0;
    }

    private static double benchmarkBigNumSequentialKaratsuba(int[] num1, int[] num2) {
        // Warmup
        for (int i = 0; i < WARMUP_RUNS; i++) {
            multiplyBigNumKaratsuba(num1, num2);
        }

        // Measure
        long totalTime = 0;
        for (int i = 0; i < MEASUREMENT_RUNS; i++) {
            long start = System.nanoTime();
            multiplyBigNumKaratsuba(num1, num2);
            totalTime += System.nanoTime() - start;
        }

        return (totalTime / MEASUREMENT_RUNS) / 1_000_000.0;
    }

    private static double benchmarkBigNumMPIRegular(int[] num1, int[] num2, int rank, int size)
            throws Exception {
        // Warmup
        for (int i = 0; i < WARMUP_RUNS; i++) {
            MPIBigNumber.multiplyRegularMPI(num1, num2, rank, size);
            MPI.COMM_WORLD.Barrier();
        }

        // Measure
        long totalTime = 0;
        for (int i = 0; i < MEASUREMENT_RUNS; i++) {
            MPI.COMM_WORLD.Barrier();
            long start = System.nanoTime();
            MPIBigNumber.multiplyRegularMPI(num1, num2, rank, size);
            MPI.COMM_WORLD.Barrier();
            totalTime += System.nanoTime() - start;
        }

        return (totalTime / MEASUREMENT_RUNS) / 1_000_000.0;
    }

    private static double benchmarkBigNumMPIKaratsuba(int[] num1, int[] num2, int rank, int size)
            throws Exception {
        // Warmup
        for (int i = 0; i < WARMUP_RUNS; i++) {
            MPIBigNumber.multiplyKaratsubaMPI(num1, num2, rank, size);
            MPI.COMM_WORLD.Barrier();
        }

        // Measure
        long totalTime = 0;
        for (int i = 0; i < MEASUREMENT_RUNS; i++) {
            MPI.COMM_WORLD.Barrier();
            long start = System.nanoTime();
            MPIBigNumber.multiplyKaratsubaMPI(num1, num2, rank, size);
            MPI.COMM_WORLD.Barrier();
            totalTime += System.nanoTime() - start;
        }

        return (totalTime / MEASUREMENT_RUNS) / 1_000_000.0;
    }

    // ========================================================================
    // SEQUENTIAL ALGORITHMS
    // ========================================================================

    private static int[] multiplySequential(int[] p1, int[] p2) {
        int n1 = p1.length;
        int n2 = p2.length;
        int[] result = new int[n1 + n2 - 1];

        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < n2; j++) {
                result[i + j] += p1[i] * p2[j];
            }
        }

        return result;
    }

    private static int[] multiplyKaratsuba(int[] p1, int[] p2) {
        int n1 = p1.length;
        int n2 = p2.length;

        if (n1 <= KARATSUBA_THRESHOLD || n2 <= KARATSUBA_THRESHOLD) {
            return multiplySequential(p1, p2);
        }

        int maxLen = Math.max(n1, n2);
        if (n1 < maxLen)
            p1 = Arrays.copyOf(p1, maxLen);
        if (n2 < maxLen)
            p2 = Arrays.copyOf(p2, maxLen);

        int mid = maxLen / 2;

        int[] p1Low = Arrays.copyOfRange(p1, 0, mid);
        int[] p1High = Arrays.copyOfRange(p1, mid, maxLen);
        int[] p2Low = Arrays.copyOfRange(p2, 0, mid);
        int[] p2High = Arrays.copyOfRange(p2, mid, maxLen);

        int[] z0 = multiplyKaratsuba(p1Low, p2Low);
        int[] z2 = multiplyKaratsuba(p1High, p2High);
        int[] z1 = multiplyKaratsuba(
                MPIPolynomial.addPolynomials(p1Low, p1High),
                MPIPolynomial.addPolynomials(p2Low, p2High));
        z1 = MPIPolynomial.subtractPolynomials(z1, z0);
        z1 = MPIPolynomial.subtractPolynomials(z1, z2);

        int resultSize = n1 + n2 - 1;
        int[] result = new int[resultSize];

        for (int i = 0; i < z0.length; i++)
            result[i] += z0[i];
        for (int i = 0; i < z1.length; i++)
            result[i + mid] += z1[i];
        for (int i = 0; i < z2.length; i++)
            result[i + 2 * mid] += z2[i];

        return result;
    }

    private static int[] multiplyBigNumKaratsuba(int[] num1, int[] num2) {
        int n1 = num1.length;
        int n2 = num2.length;

        if (n1 <= KARATSUBA_THRESHOLD || n2 <= KARATSUBA_THRESHOLD) {
            return MPIBigNumber.multiplySequential(num1, num2);
        }

        int maxLen = Math.max(n1, n2);
        if (n1 < maxLen)
            num1 = Arrays.copyOf(num1, maxLen);
        if (n2 < maxLen)
            num2 = Arrays.copyOf(num2, maxLen);

        int mid = maxLen / 2;

        int[] num1Low = Arrays.copyOfRange(num1, 0, mid);
        int[] num1High = Arrays.copyOfRange(num1, mid, maxLen);
        int[] num2Low = Arrays.copyOfRange(num2, 0, mid);
        int[] num2High = Arrays.copyOfRange(num2, mid, maxLen);

        int[] z0 = multiplyBigNumKaratsuba(num1Low, num2Low);
        int[] z2 = multiplyBigNumKaratsuba(num1High, num2High);
        int[] z1 = multiplyBigNumKaratsuba(
                MPIBigNumber.addBigNumbers(num1Low, num1High),
                MPIBigNumber.addBigNumbers(num2Low, num2High));
        z1 = MPIBigNumber.subtractBigNumbers(z1, z0);
        z1 = MPIBigNumber.subtractBigNumbers(z1, z2);

        int resultSize = n1 + n2;
        int[] result = new int[resultSize];

        for (int i = 0; i < z0.length && i < result.length; i++)
            result[i] += z0[i];
        for (int i = 0; i < z1.length && i + mid < result.length; i++)
            result[i + mid] += z1[i];
        for (int i = 0; i < z2.length && i + 2 * mid < result.length; i++)
            result[i + 2 * mid] += z2[i];

        return MPIBigNumber.normalizeResult(result);
    }

    private static void printSummary() {
        System.out.println("\n" + "═".repeat(80));
        System.out.println("                              BENCHMARK SUMMARY");
        System.out.println("═".repeat(80));
        System.out.println();
        System.out.println("Key Observations:");
        System.out.println("  • MPI overhead may cause slowdowns for small problem sizes");
        System.out.println("  • Speedup improves with larger polynomials/numbers");
        System.out.println("  • Karatsuba shows better asymptotic complexity O(n^1.585) vs O(n^2)");
        System.out.println("  • Communication costs affect parallel efficiency");
        System.out.println();
        System.out.println("For production use:");
        System.out.println("  • Use MPI for very large polynomials (degree > 10000)");
        System.out.println("  • Karatsuba beneficial when n > threshold (typically 32-64)");
        System.out.println("  • Consider hybrid approaches for optimal performance");
        System.out.println();
        System.out.println("═".repeat(80));
    }
}
