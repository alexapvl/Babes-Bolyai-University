import mpi.*;
import java.util.Arrays;
import java.util.Random;

/**
 * MPI-based distributed big number multiplication.
 * Implements both regular O(n²) and Karatsuba algorithms using MPJ Express.
 * 
 * Big numbers are represented as arrays of single digits (0-9),
 * stored in little-endian order (least significant digit first).
 */
public class MPIBigNumber {

  private static final int KARATSUBA_THRESHOLD = 32;

  public static void main(String[] args) throws Exception {
    MPI.Init(args);

    int rank = MPI.COMM_WORLD.Rank();
    int size = MPI.COMM_WORLD.Size();

    if (rank == 0) {
      System.out.println("═".repeat(80));
      System.out.println("        MPI BIG NUMBER MULTIPLICATION - LAB 7 (BONUS)");
      System.out.println("═".repeat(80));
      System.out.println("Number of MPI processes: " + size);
    }

    // Run benchmarks with different number sizes
    int[] numDigits = { 100, 500, 1000, 2000, 5000 };

    for (int digits : numDigits) {
      if (rank == 0) {
        System.out.println("\n" + "─".repeat(80));
        System.out.println("Testing big numbers with " + digits + " digits");
        System.out.println("─".repeat(80));
      }

      benchmarkBigNumberMultiplication(rank, size, digits);
      MPI.COMM_WORLD.Barrier();
    }

    MPI.Finalize();
  }

  /**
   * Benchmarks both regular and Karatsuba MPI multiplication for big numbers.
   */
  private static void benchmarkBigNumberMultiplication(int rank, int size, int numDigits)
      throws Exception {

    int[] num1 = null;
    int[] num2 = null;
    int[] seqResult = null;

    // Master generates big numbers
    if (rank == 0) {
      Random rand = new Random(42);
      num1 = new int[numDigits];
      num2 = new int[numDigits];

      // Generate random digits
      for (int i = 0; i < numDigits - 1; i++) {
        num1[i] = rand.nextInt(10);
        num2[i] = rand.nextInt(10);
      }
      // Ensure most significant digit is non-zero
      num1[numDigits - 1] = rand.nextInt(9) + 1;
      num2[numDigits - 1] = rand.nextInt(9) + 1;

      // Compute sequential result for verification
      long seqStart = System.nanoTime();
      seqResult = multiplySequential(num1, num2);
      long seqTime = System.nanoTime() - seqStart;
      System.out.printf("Sequential Regular:     %10.2f ms%n", seqTime / 1_000_000.0);
    }

    // Benchmark MPI Regular multiplication
    MPI.COMM_WORLD.Barrier();
    long mpiRegStart = System.nanoTime();
    int[] mpiRegResult = multiplyRegularMPI(num1, num2, rank, size);
    MPI.COMM_WORLD.Barrier();
    long mpiRegTime = System.nanoTime() - mpiRegStart;

    if (rank == 0) {
      System.out.printf("MPI Regular (%d procs): %10.2f ms%n", size, mpiRegTime / 1_000_000.0);
      boolean correct = compareResults(seqResult, mpiRegResult);
      System.out.println("Correctness verified: " + (correct ? "✓" : "✗"));
    }

    // Benchmark MPI Karatsuba multiplication
    MPI.COMM_WORLD.Barrier();
    long mpiKarStart = System.nanoTime();
    int[] mpiKarResult = multiplyKaratsubaMPI(num1, num2, rank, size);
    MPI.COMM_WORLD.Barrier();
    long mpiKarTime = System.nanoTime() - mpiKarStart;

    if (rank == 0) {
      System.out.printf("MPI Karatsuba (%d procs): %10.2f ms%n", size, mpiKarTime / 1_000_000.0);
      boolean correct = compareResults(seqResult, mpiKarResult);
      System.out.println("Correctness verified: " + (correct ? "✓" : "✗"));
    }
  }

  // ========================================================================
  // REGULAR O(n²) MULTIPLICATION - MPI DISTRIBUTED
  // ========================================================================

  /**
   * MPI-distributed regular big number multiplication.
   * 
   * Distribution Strategy:
   * 1. Master broadcasts both numbers to all workers
   * 2. Each worker computes partial products for a range of digits
   * 3. Workers send partial results back to master
   * 4. Master reduces (sums) all partial results and handles carries
   */
  public static int[] multiplyRegularMPI(int[] num1, int[] num2, int rank, int size)
      throws Exception {

    // Broadcast number sizes
    int[] sizes = new int[2];
    if (rank == 0) {
      sizes[0] = num1.length;
      sizes[1] = num2.length;
    }
    MPI.COMM_WORLD.Bcast(sizes, 0, 2, MPI.INT, 0);

    int n1 = sizes[0];
    int n2 = sizes[1];
    int resultSize = n1 + n2;

    // Allocate arrays on non-master processes
    if (rank != 0) {
      num1 = new int[n1];
      num2 = new int[n2];
    }

    // Broadcast numbers to all processes
    MPI.COMM_WORLD.Bcast(num1, 0, n1, MPI.INT, 0);
    MPI.COMM_WORLD.Bcast(num2, 0, n2, MPI.INT, 0);

    // Calculate work distribution
    int chunkSize = (n1 + size - 1) / size;
    int start = rank * chunkSize;
    int end = Math.min(start + chunkSize, n1);

    // Compute local partial result (without carry propagation)
    int[] localResult = new int[resultSize];

    for (int i = start; i < end; i++) {
      for (int j = 0; j < n2; j++) {
        localResult[i + j] += num1[i] * num2[j];
      }
    }

    // Reduce all partial results to master
    // Note: MPJ Express requires all processes to have valid receive buffer
    int[] globalResult = new int[resultSize];

    MPI.COMM_WORLD.Reduce(localResult, 0, globalResult, 0, resultSize, MPI.INT, MPI.SUM, 0);

    // Master handles carry propagation and returns result
    if (rank == 0) {
      return normalizeResult(globalResult);
    }
    return null;
  }

  // ========================================================================
  // KARATSUBA O(n^1.585) MULTIPLICATION - MPI DISTRIBUTED
  // ========================================================================

  /**
   * MPI-distributed Karatsuba big number multiplication.
   * 
   * Distribution Strategy:
   * Similar to polynomial Karatsuba, but with carry handling for big numbers.
   */
  public static int[] multiplyKaratsubaMPI(int[] num1, int[] num2, int rank, int size)
      throws Exception {

    // Broadcast number sizes
    int[] sizes = new int[2];
    if (rank == 0) {
      sizes[0] = num1.length;
      sizes[1] = num2.length;
    }
    MPI.COMM_WORLD.Bcast(sizes, 0, 2, MPI.INT, 0);

    int n1 = sizes[0];
    int n2 = sizes[1];

    // Allocate arrays on non-master processes
    if (rank != 0) {
      num1 = new int[n1];
      num2 = new int[n2];
    }

    // Broadcast numbers to all processes
    MPI.COMM_WORLD.Bcast(num1, 0, n1, MPI.INT, 0);
    MPI.COMM_WORLD.Bcast(num2, 0, n2, MPI.INT, 0);

    // For small numbers or insufficient processes, use regular MPI multiplication
    int maxLen = Math.max(n1, n2);
    if (maxLen <= KARATSUBA_THRESHOLD || size < 3) {
      return multiplyRegularMPI(num1, num2, rank, size);
    }

    // Pad numbers to same size
    if (n1 < maxLen) {
      num1 = Arrays.copyOf(num1, maxLen);
    }
    if (n2 < maxLen) {
      num2 = Arrays.copyOf(num2, maxLen);
    }

    // Split numbers
    int mid = maxLen / 2;

    int[] num1Low = Arrays.copyOfRange(num1, 0, mid);
    int[] num1High = Arrays.copyOfRange(num1, mid, maxLen);
    int[] num2Low = Arrays.copyOfRange(num2, 0, mid);
    int[] num2High = Arrays.copyOfRange(num2, mid, maxLen);

    // Compute sums for z1
    int[] num1Sum = addBigNumbers(num1Low, num1High);
    int[] num2Sum = addBigNumbers(num2Low, num2High);

    // Distribute the three multiplications across processes
    int groupSize = size / 3;
    int remainder = size % 3;

    int[] z0 = null, z1 = null, z2 = null;

    // Assign processes to sub-problems
    int group;
    if (rank < groupSize + (remainder > 0 ? 1 : 0)) {
      group = 0; // z0
    } else if (rank < 2 * groupSize + (remainder > 1 ? 2 : (remainder > 0 ? 1 : 0))) {
      group = 1; // z2
    } else {
      group = 2; // z1
    }

    // Each process computes based on its group assignment
    int[] localZ0 = null, localZ1 = null, localZ2 = null;

    if (group == 0) {
      localZ0 = multiplySequential(num1Low, num2Low);
    } else if (group == 1) {
      localZ2 = multiplySequential(num1High, num2High);
    } else {
      localZ1 = multiplySequential(num1Sum, num2Sum);
    }

    // Gather results at master
    // Use fixed max sizes to avoid array bounds issues with normalized results
    int resultSize = n1 + n2;
    int z0MaxSize = 2 * mid;
    int z2MaxSize = 2 * (maxLen - mid);
    int z1MaxSize = num1Sum.length + num2Sum.length;

    // Pad local results to max size for consistent communication
    if (localZ0 != null && localZ0.length < z0MaxSize) {
      localZ0 = Arrays.copyOf(localZ0, z0MaxSize);
    }
    if (localZ2 != null && localZ2.length < z2MaxSize) {
      localZ2 = Arrays.copyOf(localZ2, z2MaxSize);
    }
    if (localZ1 != null && localZ1.length < z1MaxSize) {
      localZ1 = Arrays.copyOf(localZ1, z1MaxSize);
    }

    if (rank == 0) {
      z0 = localZ0 != null ? localZ0 : new int[z0MaxSize];
      z2 = new int[z2MaxSize];
      z1 = new int[z1MaxSize];

      int z2Sender = groupSize + (remainder > 0 ? 1 : 0);
      int z1Sender = 2 * groupSize + (remainder > 1 ? 2 : (remainder > 0 ? 1 : 0));

      if (z2Sender < size) {
        MPI.COMM_WORLD.Recv(z2, 0, z2MaxSize, MPI.INT, z2Sender, 0);
      } else {
        z2 = multiplySequential(num1High, num2High);
        if (z2.length < z2MaxSize)
          z2 = Arrays.copyOf(z2, z2MaxSize);
      }

      if (z1Sender < size) {
        MPI.COMM_WORLD.Recv(z1, 0, z1MaxSize, MPI.INT, z1Sender, 1);
      } else {
        z1 = multiplySequential(num1Sum, num2Sum);
        if (z1.length < z1MaxSize)
          z1 = Arrays.copyOf(z1, z1MaxSize);
      }

    } else {
      if (group == 1 && rank == groupSize + (remainder > 0 ? 1 : 0)) {
        MPI.COMM_WORLD.Send(localZ2, 0, z2MaxSize, MPI.INT, 0, 0);
      } else if (group == 2 && rank == 2 * groupSize + (remainder > 1 ? 2 : (remainder > 0 ? 1 : 0))) {
        MPI.COMM_WORLD.Send(localZ1, 0, z1MaxSize, MPI.INT, 0, 1);
      }
    }

    // Master combines results
    if (rank == 0) {
      // z1 = z1 - z0 - z2
      z1 = subtractBigNumbers(z1, z0);
      z1 = subtractBigNumbers(z1, z2);

      // result = z0 + z1*10^mid + z2*10^(2*mid)
      int[] result = new int[resultSize];

      // Add z0
      for (int i = 0; i < z0.length && i < result.length; i++) {
        result[i] += z0[i];
      }

      // Add z1 shifted by mid
      for (int i = 0; i < z1.length && i + mid < result.length; i++) {
        result[i + mid] += z1[i];
      }

      // Add z2 shifted by 2*mid
      for (int i = 0; i < z2.length && i + 2 * mid < result.length; i++) {
        result[i + 2 * mid] += z2[i];
      }

      return normalizeResult(result);
    }

    return null;
  }

  // ========================================================================
  // HELPER METHODS
  // ========================================================================

  /**
   * Sequential big number multiplication (grade-school algorithm).
   */
  public static int[] multiplySequential(int[] num1, int[] num2) {
    int n1 = num1.length;
    int n2 = num2.length;
    int[] result = new int[n1 + n2];

    for (int i = 0; i < n1; i++) {
      for (int j = 0; j < n2; j++) {
        result[i + j] += num1[i] * num2[j];
      }
    }

    return normalizeResult(result);
  }

  /**
   * Normalizes result by propagating carries.
   */
  public static int[] normalizeResult(int[] result) {
    int[] normalized = Arrays.copyOf(result, result.length);

    for (int i = 0; i < normalized.length - 1; i++) {
      if (normalized[i] >= 10) {
        normalized[i + 1] += normalized[i] / 10;
        normalized[i] %= 10;
      } else if (normalized[i] < 0) {
        int borrow = (-normalized[i] + 9) / 10;
        normalized[i] += borrow * 10;
        normalized[i + 1] -= borrow;
      }
    }

    // Remove leading zeros
    int size = normalized.length;
    while (size > 1 && normalized[size - 1] == 0) {
      size--;
    }

    return Arrays.copyOf(normalized, size);
  }

  /**
   * Adds two big numbers.
   */
  public static int[] addBigNumbers(int[] num1, int[] num2) {
    int maxLen = Math.max(num1.length, num2.length);
    int[] result = new int[maxLen + 1];

    for (int i = 0; i < maxLen; i++) {
      int d1 = i < num1.length ? num1[i] : 0;
      int d2 = i < num2.length ? num2[i] : 0;
      result[i] += d1 + d2;
      if (result[i] >= 10) {
        result[i + 1] += result[i] / 10;
        result[i] %= 10;
      }
    }

    // Remove leading zeros
    int size = result.length;
    while (size > 1 && result[size - 1] == 0) {
      size--;
    }

    return Arrays.copyOf(result, size);
  }

  /**
   * Subtracts num2 from num1.
   */
  public static int[] subtractBigNumbers(int[] num1, int[] num2) {
    int maxLen = Math.max(num1.length, num2.length);
    int[] result = new int[maxLen];

    for (int i = 0; i < maxLen; i++) {
      int d1 = i < num1.length ? num1[i] : 0;
      int d2 = i < num2.length ? num2[i] : 0;
      result[i] = d1 - d2;
    }

    return result; // Don't normalize here - may have intermediate negatives
  }

  /**
   * Compares two results for equality (ignoring trailing zeros).
   */
  public static boolean compareResults(int[] r1, int[] r2) {
    if (r1 == null || r2 == null)
      return false;

    int len1 = r1.length;
    int len2 = r2.length;

    // Remove trailing zeros for comparison
    while (len1 > 1 && r1[len1 - 1] == 0)
      len1--;
    while (len2 > 1 && r2[len2 - 1] == 0)
      len2--;

    if (len1 != len2)
      return false;

    for (int i = 0; i < len1; i++) {
      if (r1[i] != r2[i])
        return false;
    }

    return true;
  }
}
