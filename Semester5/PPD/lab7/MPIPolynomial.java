import mpi.*;
import java.util.Arrays;
import java.util.Random;

/**
 * MPI-based distributed polynomial multiplication.
 * Implements both regular O(n²) and Karatsuba algorithms using MPJ Express.
 * 
 * Distribution Strategy:
 * - Master (rank 0) generates polynomials and coordinates distribution
 * - Workers receive chunks of coefficients and compute partial products
 * - Results are gathered back at master using MPI collective operations
 */
public class MPIPolynomial {

  private static final int KARATSUBA_THRESHOLD = 32;

  public static void main(String[] args) throws Exception {
    MPI.Init(args);

    int rank = MPI.COMM_WORLD.Rank();
    int size = MPI.COMM_WORLD.Size();

    if (rank == 0) {
      System.out.println("═".repeat(80));
      System.out.println("        MPI POLYNOMIAL MULTIPLICATION - LAB 7");
      System.out.println("═".repeat(80));
      System.out.println("Number of MPI processes: " + size);
    }

    // Run benchmarks with different polynomial sizes
    int[] degrees = { 100, 500, 1000, 2000, 5000 };

    for (int degree : degrees) {
      if (rank == 0) {
        System.out.println("\n" + "─".repeat(80));
        System.out.println("Testing polynomials of degree: " + degree);
        System.out.println("─".repeat(80));
      }

      benchmarkPolynomialMultiplication(rank, size, degree);
      MPI.COMM_WORLD.Barrier();
    }

    MPI.Finalize();
  }

  /**
   * Benchmarks both regular and Karatsuba MPI multiplication.
   */
  private static void benchmarkPolynomialMultiplication(int rank, int size, int degree)
      throws Exception {

    int[] p1Coeffs = null;
    int[] p2Coeffs = null;
    int[] seqResult = null;

    // Master generates polynomials
    if (rank == 0) {
      Random rand = new Random(42); // Fixed seed for reproducibility
      p1Coeffs = new int[degree + 1];
      p2Coeffs = new int[degree + 1];

      for (int i = 0; i <= degree; i++) {
        p1Coeffs[i] = rand.nextInt(100);
        p2Coeffs[i] = rand.nextInt(100);
      }
      p1Coeffs[degree] = Math.max(1, p1Coeffs[degree]); // Ensure non-zero leading coeff
      p2Coeffs[degree] = Math.max(1, p2Coeffs[degree]);

      // Compute sequential result for verification
      long seqStart = System.nanoTime();
      seqResult = multiplySequential(p1Coeffs, p2Coeffs);
      long seqTime = System.nanoTime() - seqStart;
      System.out.printf("Sequential Regular:     %10.2f ms%n", seqTime / 1_000_000.0);
    }

    // Benchmark MPI Regular multiplication
    MPI.COMM_WORLD.Barrier();
    long mpiRegStart = System.nanoTime();
    int[] mpiRegResult = multiplyRegularMPI(p1Coeffs, p2Coeffs, rank, size);
    MPI.COMM_WORLD.Barrier();
    long mpiRegTime = System.nanoTime() - mpiRegStart;

    if (rank == 0) {
      System.out.printf("MPI Regular (%d procs): %10.2f ms%n", size, mpiRegTime / 1_000_000.0);
      boolean correct = Arrays.equals(seqResult, mpiRegResult);
      System.out.println("Correctness verified: " + (correct ? "✓" : "✗"));
    }

    // Benchmark MPI Karatsuba multiplication
    MPI.COMM_WORLD.Barrier();
    long mpiKarStart = System.nanoTime();
    int[] mpiKarResult = multiplyKaratsubaMPI(p1Coeffs, p2Coeffs, rank, size);
    MPI.COMM_WORLD.Barrier();
    long mpiKarTime = System.nanoTime() - mpiKarStart;

    if (rank == 0) {
      System.out.printf("MPI Karatsuba (%d procs): %10.2f ms%n", size, mpiKarTime / 1_000_000.0);
      boolean correct = Arrays.equals(seqResult, mpiKarResult);
      System.out.println("Correctness verified: " + (correct ? "✓" : "✗"));
    }
  }

  // ========================================================================
  // REGULAR O(n²) MULTIPLICATION - MPI DISTRIBUTED
  // ========================================================================

  /**
   * MPI-distributed regular polynomial multiplication.
   * 
   * Distribution Strategy:
   * 1. Master broadcasts both polynomials to all workers
   * 2. Each worker computes partial products for a range of coefficients
   * 3. Workers send partial results back to master
   * 4. Master reduces (sums) all partial results
   * 
   * @param p1   First polynomial coefficients (only valid on rank 0)
   * @param p2   Second polynomial coefficients (only valid on rank 0)
   * @param rank Current process rank
   * @param size Total number of processes
   * @return Result polynomial coefficients (only valid on rank 0)
   */
  public static int[] multiplyRegularMPI(int[] p1, int[] p2, int rank, int size)
      throws Exception {

    // Broadcast polynomial sizes
    int[] sizes = new int[2];
    if (rank == 0) {
      sizes[0] = p1.length;
      sizes[1] = p2.length;
    }
    MPI.COMM_WORLD.Bcast(sizes, 0, 2, MPI.INT, 0);

    int n1 = sizes[0];
    int n2 = sizes[1];
    int resultSize = n1 + n2 - 1;

    // Allocate arrays on non-master processes
    if (rank != 0) {
      p1 = new int[n1];
      p2 = new int[n2];
    }

    // Broadcast polynomials to all processes
    MPI.COMM_WORLD.Bcast(p1, 0, n1, MPI.INT, 0);
    MPI.COMM_WORLD.Bcast(p2, 0, n2, MPI.INT, 0);

    // Calculate work distribution - each process handles a range of p1 coefficients
    int chunkSize = (n1 + size - 1) / size;
    int start = rank * chunkSize;
    int end = Math.min(start + chunkSize, n1);

    // Compute local partial result
    int[] localResult = new int[resultSize];

    for (int i = start; i < end; i++) {
      for (int j = 0; j < n2; j++) {
        localResult[i + j] += p1[i] * p2[j];
      }
    }

    // Reduce all partial results to master
    // Note: MPJ Express requires all processes to have valid receive buffer
    int[] globalResult = new int[resultSize];

    MPI.COMM_WORLD.Reduce(localResult, 0, globalResult, 0, resultSize, MPI.INT, MPI.SUM, 0);

    // Only master returns valid result, workers return null
    return rank == 0 ? globalResult : null;
  }

  // ========================================================================
  // KARATSUBA O(n^1.585) MULTIPLICATION - MPI DISTRIBUTED
  // ========================================================================

  /**
   * MPI-distributed Karatsuba polynomial multiplication.
   * 
   * Distribution Strategy:
   * 1. Master broadcasts both polynomials to all workers
   * 2. For the three recursive multiplications (z0, z1, z2):
   * - If we have enough processes, assign each to different process groups
   * - Otherwise, each process computes all three recursively
   * 3. Results are combined at the master
   * 
   * For educational purposes, this implementation uses a simpler approach:
   * - The Karatsuba decomposition happens at the master
   * - Sub-multiplications are distributed to worker processes
   * - Workers compute their assigned sub-problems using regular multiplication
   * 
   * @param p1   First polynomial coefficients (only valid on rank 0)
   * @param p2   Second polynomial coefficients (only valid on rank 0)
   * @param rank Current process rank
   * @param size Total number of processes
   * @return Result polynomial coefficients (only valid on rank 0)
   */
  public static int[] multiplyKaratsubaMPI(int[] p1, int[] p2, int rank, int size)
      throws Exception {

    // Broadcast polynomial sizes
    int[] sizes = new int[2];
    if (rank == 0) {
      sizes[0] = p1.length;
      sizes[1] = p2.length;
    }
    MPI.COMM_WORLD.Bcast(sizes, 0, 2, MPI.INT, 0);

    int n1 = sizes[0];
    int n2 = sizes[1];

    // Allocate arrays on non-master processes
    if (rank != 0) {
      p1 = new int[n1];
      p2 = new int[n2];
    }

    // Broadcast polynomials to all processes
    MPI.COMM_WORLD.Bcast(p1, 0, n1, MPI.INT, 0);
    MPI.COMM_WORLD.Bcast(p2, 0, n2, MPI.INT, 0);

    // For small polynomials or insufficient processes, use regular MPI
    // multiplication
    int maxLen = Math.max(n1, n2);
    if (maxLen <= KARATSUBA_THRESHOLD || size < 3) {
      return multiplyRegularMPI(p1, p2, rank, size);
    }

    // Pad polynomials to same size
    if (n1 < maxLen) {
      p1 = Arrays.copyOf(p1, maxLen);
    }
    if (n2 < maxLen) {
      p2 = Arrays.copyOf(p2, maxLen);
    }

    // Split polynomials
    int mid = maxLen / 2;

    int[] p1Low = Arrays.copyOfRange(p1, 0, mid);
    int[] p1High = Arrays.copyOfRange(p1, mid, maxLen);
    int[] p2Low = Arrays.copyOfRange(p2, 0, mid);
    int[] p2High = Arrays.copyOfRange(p2, mid, maxLen);

    // Compute sums for z1
    int[] p1Sum = addPolynomials(p1Low, p1High);
    int[] p2Sum = addPolynomials(p2Low, p2High);

    // Distribute the three multiplications across processes
    // Process groups: ranks 0 to size/3-1 compute z0
    // ranks size/3 to 2*size/3-1 compute z2
    // ranks 2*size/3 to size-1 compute z1

    int groupSize = size / 3;
    int remainder = size % 3;

    int[] z0 = null, z1 = null, z2 = null;

    // Assign processes to sub-problems
    int group;
    if (rank < groupSize + (remainder > 0 ? 1 : 0)) {
      group = 0; // z0: p1Low * p2Low
    } else if (rank < 2 * groupSize + (remainder > 1 ? 2 : (remainder > 0 ? 1 : 0))) {
      group = 1; // z2: p1High * p2High
    } else {
      group = 2; // z1: p1Sum * p2Sum
    }

    // Each process computes based on its group assignment
    int[] localZ0 = null, localZ1 = null, localZ2 = null;

    if (group == 0) {
      localZ0 = multiplySequential(p1Low, p2Low);
    } else if (group == 1) {
      localZ2 = multiplySequential(p1High, p2High);
    } else {
      localZ1 = multiplySequential(p1Sum, p2Sum);
    }

    // Gather results at master using point-to-point communication
    int resultSize = n1 + n2 - 1;
    int z0Size = 2 * mid - 1;
    int z2Size = 2 * (maxLen - mid) - 1;
    int z1Size = p1Sum.length + p2Sum.length - 1;

    if (rank == 0) {
      // Master process - collect results
      z0 = localZ0 != null ? localZ0 : new int[z0Size];
      z2 = new int[z2Size];
      z1 = new int[z1Size];

      // Find first process in each group and receive from them
      int z2Sender = groupSize + (remainder > 0 ? 1 : 0);
      int z1Sender = 2 * groupSize + (remainder > 1 ? 2 : (remainder > 0 ? 1 : 0));

      if (z2Sender < size) {
        MPI.COMM_WORLD.Recv(z2, 0, z2Size, MPI.INT, z2Sender, 0);
      } else {
        z2 = multiplySequential(p1High, p2High);
      }

      if (z1Sender < size) {
        MPI.COMM_WORLD.Recv(z1, 0, z1Size, MPI.INT, z1Sender, 1);
      } else {
        z1 = multiplySequential(p1Sum, p2Sum);
      }

    } else {
      // Worker processes - send their results to master
      if (group == 1 && rank == groupSize + (remainder > 0 ? 1 : 0)) {
        MPI.COMM_WORLD.Send(localZ2, 0, z2Size, MPI.INT, 0, 0);
      } else if (group == 2 && rank == 2 * groupSize + (remainder > 1 ? 2 : (remainder > 0 ? 1 : 0))) {
        MPI.COMM_WORLD.Send(localZ1, 0, z1Size, MPI.INT, 0, 1);
      }
    }

    // Master combines results
    if (rank == 0) {
      // z1 = z1 - z0 - z2
      z1 = subtractPolynomials(z1, z0);
      z1 = subtractPolynomials(z1, z2);

      // result = z0 + z1*x^mid + z2*x^(2*mid)
      int[] result = new int[resultSize];

      // Add z0
      for (int i = 0; i < z0.length; i++) {
        result[i] += z0[i];
      }

      // Add z1 shifted by mid
      for (int i = 0; i < z1.length; i++) {
        result[i + mid] += z1[i];
      }

      // Add z2 shifted by 2*mid
      for (int i = 0; i < z2.length; i++) {
        result[i + 2 * mid] += z2[i];
      }

      return result;
    }

    return null;
  }

  // ========================================================================
  // HELPER METHODS
  // ========================================================================

  /**
   * Sequential polynomial multiplication (for reference and sub-problems).
   */
  public static int[] multiplySequential(int[] p1, int[] p2) {
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

  /**
   * Adds two polynomial coefficient arrays.
   */
  public static int[] addPolynomials(int[] p1, int[] p2) {
    int maxLen = Math.max(p1.length, p2.length);
    int[] result = new int[maxLen];

    for (int i = 0; i < maxLen; i++) {
      int c1 = i < p1.length ? p1[i] : 0;
      int c2 = i < p2.length ? p2[i] : 0;
      result[i] = c1 + c2;
    }

    return result;
  }

  /**
   * Subtracts p2 from p1.
   */
  public static int[] subtractPolynomials(int[] p1, int[] p2) {
    int maxLen = Math.max(p1.length, p2.length);
    int[] result = new int[maxLen];

    for (int i = 0; i < maxLen; i++) {
      int c1 = i < p1.length ? p1[i] : 0;
      int c2 = i < p2.length ? p2[i] : 0;
      result[i] = c1 - c2;
    }

    return result;
  }
}
