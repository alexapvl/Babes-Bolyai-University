import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.Callable;
import java.util.concurrent.RecursiveTask;
import java.util.concurrent.ForkJoinPool;

/**
 * Karatsuba polynomial multiplication algorithms.
 * Time complexity: O(n^log2(3)) ≈ O(n^1.585)
 */
public class KaratsubaMultiplication {

  // Threshold below which we switch to sequential regular multiplication
  private static final int THRESHOLD = 64;

  /**
   * Sequential implementation of Karatsuba multiplication.
   * 
   * Algorithm:
   * Let P(x) = P_high * x^m + P_low
   * Let Q(x) = Q_high * x^m + Q_low
   * 
   * Then P(x) * Q(x) = z2 * x^(2m) + z1 * x^m + z0
   * where:
   * z0 = P_low * Q_low
   * z2 = P_high * Q_high
   * z1 = (P_low + P_high) * (Q_low + Q_high) - z0 - z2
   * 
   * This reduces 3 multiplications instead of 4.
   * 
   * @param p1 First polynomial
   * @param p2 Second polynomial
   * @return The product polynomial
   */
  public static Polynomial multiplySequential(Polynomial p1, Polynomial p2) {
    // Base case: use regular multiplication for small polynomials
    if (p1.length() <= THRESHOLD || p2.length() <= THRESHOLD) {
      return RegularMultiplication.multiplySequential(p1, p2);
    }

    // Make polynomials same size by padding with zeros
    int maxLen = Math.max(p1.length(), p2.length());
    if (p1.length() < maxLen) {
      int[] newCoeffs = new int[maxLen];
      System.arraycopy(p1.getCoefficients(), 0, newCoeffs, 0, p1.length());
      p1 = new Polynomial(newCoeffs);
    }
    if (p2.length() < maxLen) {
      int[] newCoeffs = new int[maxLen];
      System.arraycopy(p2.getCoefficients(), 0, newCoeffs, 0, p2.length());
      p2 = new Polynomial(newCoeffs);
    }

    // Split polynomials in half
    int mid = maxLen / 2;

    Polynomial p1Low = p1.subPolynomial(0, mid);
    Polynomial p1High = p1.subPolynomial(mid, maxLen);
    Polynomial p2Low = p2.subPolynomial(0, mid);
    Polynomial p2High = p2.subPolynomial(mid, maxLen);

    // Three recursive multiplications
    Polynomial z0 = multiplySequential(p1Low, p2Low);
    Polynomial z2 = multiplySequential(p1High, p2High);
    Polynomial z1 = multiplySequential(
        Polynomial.add(p1Low, p1High),
        Polynomial.add(p2Low, p2High));
    z1 = Polynomial.subtract(z1, z0);
    z1 = Polynomial.subtract(z1, z2);

    // Combine results: z2 * x^(2*mid) + z1 * x^mid + z0
    Polynomial result = z0;
    result = Polynomial.add(result, z1.shift(mid));
    result = Polynomial.add(result, z2.shift(2 * mid));

    return result;
  }

  /**
   * Parallel implementation of Karatsuba multiplication.
   * 
   * Synchronization: Uses ForkJoinPool with RecursiveTask to parallelize
   * the three recursive multiplications. ForkJoinPool is specifically designed
   * for recursive parallel tasks and uses work-stealing to avoid deadlocks.
   * 
   * The parallelization depth is controlled to avoid creating too many tasks.
   * After a certain depth, we switch to sequential execution.
   * 
   * @param p1         First polynomial
   * @param p2         Second polynomial
   * @param numThreads Number of threads to use
   * @return The product polynomial
   */
  public static Polynomial multiplyParallel(Polynomial p1, Polynomial p2, int numThreads) {
    ForkJoinPool pool = new ForkJoinPool(numThreads);
    try {
      return pool.invoke(new KaratsubaTask(p1, p2, 0));
    } finally {
      pool.shutdown();
    }
  }

  /**
   * RecursiveTask for parallel Karatsuba multiplication using ForkJoinPool.
   * This avoids the thread pool exhaustion problem of fixed thread pools.
   */
  private static class KaratsubaTask extends RecursiveTask<Polynomial> {
    private final Polynomial p1;
    private final Polynomial p2;
    private final int depth;
    private static final int MAX_PARALLEL_DEPTH = 3;

    public KaratsubaTask(Polynomial p1, Polynomial p2, int depth) {
      this.p1 = p1;
      this.p2 = p2;
      this.depth = depth;
    }

    @Override
    protected Polynomial compute() {
      // Base case: use regular multiplication for small polynomials
      if (p1.length() <= THRESHOLD || p2.length() <= THRESHOLD) {
        return RegularMultiplication.multiplySequential(p1, p2);
      }

      // Limit parallelization depth to avoid too many tasks
      if (depth > MAX_PARALLEL_DEPTH) {
        return multiplySequential(p1, p2);
      }

      // Make polynomials same size
      Polynomial p1Padded = p1;
      Polynomial p2Padded = p2;
      int maxLen = Math.max(p1.length(), p2.length());

      if (p1.length() < maxLen) {
        int[] newCoeffs = new int[maxLen];
        System.arraycopy(p1.getCoefficients(), 0, newCoeffs, 0, p1.length());
        p1Padded = new Polynomial(newCoeffs);
      }
      if (p2.length() < maxLen) {
        int[] newCoeffs = new int[maxLen];
        System.arraycopy(p2.getCoefficients(), 0, newCoeffs, 0, p2.length());
        p2Padded = new Polynomial(newCoeffs);
      }

      // Split polynomials
      int mid = maxLen / 2;
      Polynomial p1Low = p1Padded.subPolynomial(0, mid);
      Polynomial p1High = p1Padded.subPolynomial(mid, maxLen);
      Polynomial p2Low = p2Padded.subPolynomial(0, mid);
      Polynomial p2High = p2Padded.subPolynomial(mid, maxLen);

      // Fork three recursive tasks
      KaratsubaTask task0 = new KaratsubaTask(p1Low, p2Low, depth + 1);
      KaratsubaTask task2 = new KaratsubaTask(p1High, p2High, depth + 1);
      KaratsubaTask task1 = new KaratsubaTask(
          Polynomial.add(p1Low, p1High),
          Polynomial.add(p2Low, p2High),
          depth + 1);

      // Fork the first two tasks
      task0.fork();
      task2.fork();

      // Compute the third task in the current thread
      Polynomial z1 = task1.compute();

      // Join the forked tasks
      Polynomial z0 = task0.join();
      Polynomial z2 = task2.join();

      // Combine results
      z1 = Polynomial.subtract(z1, z0);
      z1 = Polynomial.subtract(z1, z2);

      Polynomial result = z0;
      result = Polynomial.add(result, z1.shift(mid));
      result = Polynomial.add(result, z2.shift(2 * mid));

      return result;
    }
  }
}
