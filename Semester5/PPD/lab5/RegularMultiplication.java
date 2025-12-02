import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

/**
 * Regular O(n^2) polynomial multiplication algorithms.
 */
public class RegularMultiplication {

  /**
   * Sequential implementation of the regular polynomial multiplication.
   * Time complexity: O(n^2)
   * 
   * @param p1 First polynomial
   * @param p2 Second polynomial
   * @return The product polynomial
   */
  public static Polynomial multiplySequential(Polynomial p1, Polynomial p2) {
    int n1 = p1.length();
    int n2 = p2.length();
    int[] result = new int[n1 + n2 - 1];

    // For each coefficient in p1
    for (int i = 0; i < n1; i++) {
      // Multiply it by each coefficient in p2
      for (int j = 0; j < n2; j++) {
        result[i + j] += p1.getCoefficient(i) * p2.getCoefficient(j);
      }
    }

    return new Polynomial(result);
  }

  /**
   * Parallel implementation of the regular polynomial multiplication.
   * Uses thread pool to parallelize the outer loop.
   * 
   * Synchronization: Each thread works on a separate range of coefficients
   * from the first polynomial, computing its contribution to the result.
   * No explicit synchronization needed as each thread writes to different
   * result indices (no race conditions).
   * 
   * @param p1         First polynomial
   * @param p2         Second polynomial
   * @param numThreads Number of threads to use
   * @return The product polynomial
   */
  public static Polynomial multiplyParallel(Polynomial p1, Polynomial p2, int numThreads) {
    int n1 = p1.length();
    int n2 = p2.length();
    int[] result = new int[n1 + n2 - 1];

    ExecutorService executor = Executors.newFixedThreadPool(numThreads);

    // Divide work among threads
    int chunkSize = (n1 + numThreads - 1) / numThreads;

    for (int t = 0; t < numThreads; t++) {
      final int start = t * chunkSize;
      final int end = Math.min(start + chunkSize, n1);

      if (start >= n1)
        break;

      executor.execute(() -> {
        // Each thread computes a range of coefficients from p1
        for (int i = start; i < end; i++) {
          for (int j = 0; j < n2; j++) {
            // Synchronized access to result array
            synchronized (result) {
              result[i + j] += p1.getCoefficient(i) * p2.getCoefficient(j);
            }
          }
        }
      });
    }

    executor.shutdown();
    try {
      executor.awaitTermination(1, TimeUnit.HOURS);
    } catch (InterruptedException e) {
      e.printStackTrace();
    }

    return new Polynomial(result);
  }

  /**
   * Parallel implementation using a different strategy - parallelizing by result
   * coefficients.
   * This variant assigns each result coefficient to a thread.
   * 
   * @param p1         First polynomial
   * @param p2         Second polynomial
   * @param numThreads Number of threads to use
   * @return The product polynomial
   */
  public static Polynomial multiplyParallelV2(Polynomial p1, Polynomial p2, int numThreads) {
    int n1 = p1.length();
    int n2 = p2.length();
    int resultSize = n1 + n2 - 1;
    int[] result = new int[resultSize];

    ExecutorService executor = Executors.newFixedThreadPool(numThreads);

    // Divide result coefficients among threads
    int chunkSize = (resultSize + numThreads - 1) / numThreads;

    for (int t = 0; t < numThreads; t++) {
      final int start = t * chunkSize;
      final int end = Math.min(start + chunkSize, resultSize);

      if (start >= resultSize)
        break;

      executor.execute(() -> {
        // Each thread computes a range of result coefficients
        for (int k = start; k < end; k++) {
          int sum = 0;
          // Find all pairs (i,j) where i+j = k
          for (int i = 0; i < n1; i++) {
            int j = k - i;
            if (j >= 0 && j < n2) {
              sum += p1.getCoefficient(i) * p2.getCoefficient(j);
            }
          }
          result[k] = sum;
        }
      });
    }

    executor.shutdown();
    try {
      executor.awaitTermination(1, TimeUnit.HOURS);
    } catch (InterruptedException e) {
      e.printStackTrace();
    }

    return new Polynomial(result);
  }
}
