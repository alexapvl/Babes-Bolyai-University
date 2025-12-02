import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.Future;
import java.util.concurrent.Callable;
import java.util.concurrent.RecursiveTask;
import java.util.concurrent.ForkJoinPool;

/**
 * Big number multiplication algorithms.
 * Implements both regular O(n^2) and Karatsuba O(n^1.585) algorithms,
 * each in sequential and parallel variants.
 */
public class BigNumberMultiplication {

    private static final int KARATSUBA_THRESHOLD = 64;

    /**
     * Sequential regular multiplication (grade-school algorithm).
     * Time complexity: O(n^2)
     */
    public static BigNumber multiplySequential(BigNumber a, BigNumber b) {
        int n1 = a.length();
        int n2 = b.length();
        int[] result = new int[n1 + n2];

        // Multiply each digit of a with each digit of b
        for (int i = 0; i < n1; i++) {
            int carry = 0;
            for (int j = 0; j < n2; j++) {
                int product = a.getDigit(i) * b.getDigit(j) + result[i + j] + carry;
                result[i + j] = product % 10;
                carry = product / 10;
            }
            if (carry > 0) {
                result[i + n2] += carry;
            }
        }

        // Handle any remaining carries
        for (int i = 0; i < result.length - 1; i++) {
            if (result[i] >= 10) {
                result[i + 1] += result[i] / 10;
                result[i] %= 10;
            }
        }

        boolean isNegative = a.isNegative() != b.isNegative();
        return new BigNumber(result, isNegative);
    }

    /**
     * Parallel regular multiplication.
     * Parallelizes the outer loop across multiple threads.
     * Each thread processes a range of digits from the first number.
     */
    public static BigNumber multiplyParallel(BigNumber a, BigNumber b, int numThreads) {
        int n1 = a.length();
        int n2 = b.length();
        int[] result = new int[n1 + n2];

        ExecutorService executor = Executors.newFixedThreadPool(numThreads);
        int chunkSize = (n1 + numThreads - 1) / numThreads;

        for (int t = 0; t < numThreads; t++) {
            final int start = t * chunkSize;
            final int end = Math.min(start + chunkSize, n1);

            if (start >= n1) break;

            executor.execute(() -> {
                for (int i = start; i < end; i++) {
                    int carry = 0;
                    for (int j = 0; j < n2; j++) {
                        int product = a.getDigit(i) * b.getDigit(j) + carry;
                        synchronized (result) {
                            product += result[i + j];
                            result[i + j] = product % 10;
                        }
                        carry = product / 10;
                    }
                    if (carry > 0) {
                        synchronized (result) {
                            result[i + n2] += carry;
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

        // Handle carries
        for (int i = 0; i < result.length - 1; i++) {
            if (result[i] >= 10) {
                result[i + 1] += result[i] / 10;
                result[i] %= 10;
            }
        }

        boolean isNegative = a.isNegative() != b.isNegative();
        return new BigNumber(result, isNegative);
    }

    /**
     * Sequential Karatsuba multiplication.
     * Time complexity: O(n^log2(3)) ≈ O(n^1.585)
     */
    public static BigNumber multiplyKaratsubaSequential(BigNumber a, BigNumber b) {
        // Base case: use regular multiplication for small numbers
        if (a.length() <= KARATSUBA_THRESHOLD || b.length() <= KARATSUBA_THRESHOLD) {
            return multiplySequential(a, b);
        }

        // Make numbers same length by padding with zeros
        int maxLen = Math.max(a.length(), b.length());
        if (a.length() < maxLen) {
            int[] newDigits = new int[maxLen];
            System.arraycopy(a.getDigits(), 0, newDigits, 0, a.length());
            a = new BigNumber(newDigits, a.isNegative());
        }
        if (b.length() < maxLen) {
            int[] newDigits = new int[maxLen];
            System.arraycopy(b.getDigits(), 0, newDigits, 0, b.length());
            b = new BigNumber(newDigits, b.isNegative());
        }

        // Split numbers in half
        int mid = maxLen / 2;

        BigNumber aLow = a.subNumber(0, mid);
        BigNumber aHigh = a.subNumber(mid, maxLen);
        BigNumber bLow = b.subNumber(0, mid);
        BigNumber bHigh = b.subNumber(mid, maxLen);

        // Three recursive multiplications
        BigNumber z0 = multiplyKaratsubaSequential(aLow, bLow);
        BigNumber z2 = multiplyKaratsubaSequential(aHigh, bHigh);
        BigNumber z1 = multiplyKaratsubaSequential(
            BigNumber.add(aLow, aHigh),
            BigNumber.add(bLow, bHigh)
        );
        z1 = BigNumber.subtract(z1, z0);
        z1 = BigNumber.subtract(z1, z2);

        // Combine: z2 * 10^(2*mid) + z1 * 10^mid + z0
        BigNumber result = z0;
        result = BigNumber.add(result, z1.shift(mid));
        result = BigNumber.add(result, z2.shift(2 * mid));

        boolean isNegative = a.isNegative() != b.isNegative();
        return new BigNumber(result.getDigits(), isNegative);
    }

    /**
     * Parallel Karatsuba multiplication.
     * Uses ForkJoinPool to parallelize the three recursive multiplications.
     * This avoids thread pool exhaustion that can occur with fixed thread pools.
     */
    public static BigNumber multiplyKaratsubaParallel(BigNumber a, BigNumber b, int numThreads) {
        ForkJoinPool pool = new ForkJoinPool(numThreads);
        try {
            return pool.invoke(new KaratsubaTask(a, b, 0));
        } finally {
            pool.shutdown();
        }
    }

    /**
     * RecursiveTask for parallel Karatsuba multiplication using ForkJoinPool.
     */
    private static class KaratsubaTask extends RecursiveTask<BigNumber> {
        private final BigNumber a;
        private final BigNumber b;
        private final int depth;
        private static final int MAX_PARALLEL_DEPTH = 3;

        public KaratsubaTask(BigNumber a, BigNumber b, int depth) {
            this.a = a;
            this.b = b;
            this.depth = depth;
        }

        @Override
        protected BigNumber compute() {
            // Base case
            if (a.length() <= KARATSUBA_THRESHOLD || b.length() <= KARATSUBA_THRESHOLD) {
                return multiplySequential(a, b);
            }

            // Limit parallelization depth
            if (depth > MAX_PARALLEL_DEPTH) {
                return multiplyKaratsubaSequential(a, b);
            }

            // Make same length
            BigNumber aPadded = a;
            BigNumber bPadded = b;
            int maxLen = Math.max(a.length(), b.length());
            
            if (a.length() < maxLen) {
                int[] newDigits = new int[maxLen];
                System.arraycopy(a.getDigits(), 0, newDigits, 0, a.length());
                aPadded = new BigNumber(newDigits, a.isNegative());
            }
            if (b.length() < maxLen) {
                int[] newDigits = new int[maxLen];
                System.arraycopy(b.getDigits(), 0, newDigits, 0, b.length());
                bPadded = new BigNumber(newDigits, b.isNegative());
            }

            // Split
            int mid = maxLen / 2;
            BigNumber aLow = aPadded.subNumber(0, mid);
            BigNumber aHigh = aPadded.subNumber(mid, maxLen);
            BigNumber bLow = bPadded.subNumber(0, mid);
            BigNumber bHigh = bPadded.subNumber(mid, maxLen);

            // Fork three recursive tasks
            KaratsubaTask task0 = new KaratsubaTask(aLow, bLow, depth + 1);
            KaratsubaTask task2 = new KaratsubaTask(aHigh, bHigh, depth + 1);
            KaratsubaTask task1 = new KaratsubaTask(
                BigNumber.add(aLow, aHigh),
                BigNumber.add(bLow, bHigh),
                depth + 1);

            // Fork the first two tasks
            task0.fork();
            task2.fork();

            // Compute the third task in the current thread
            BigNumber z1 = task1.compute();

            // Join the forked tasks
            BigNumber z0 = task0.join();
            BigNumber z2 = task2.join();

            // Combine results
            z1 = BigNumber.subtract(z1, z0);
            z1 = BigNumber.subtract(z1, z2);

            BigNumber result = z0;
            result = BigNumber.add(result, z1.shift(mid));
            result = BigNumber.add(result, z2.shift(2 * mid));

            boolean isNegative = a.isNegative() != b.isNegative();
            return new BigNumber(result.getDigits(), isNegative);
        }
    }
}

