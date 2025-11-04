public class MatrixMultiplication {
    private final int[][] matrixA;
    private final int[][] matrixB;
    private final int[][] result;
    private final int rowsA;
    private final int colsA;
    private final int colsB;
    private final boolean debug;

    public MatrixMultiplication(int[][] matrixA, int[][] matrixB, boolean debug) {
        this.matrixA = matrixA;
        this.matrixB = matrixB;
        this.rowsA = matrixA.length;
        this.colsA = matrixA[0].length;
        this.colsB = matrixB[0].length;
        this.result = new int[rowsA][colsB];
        this.debug = debug;
    }

    /**
     * Computes a single element of the resulting matrix.
     * This is the core computation function.
     */
    private void computeElement(int row, int col, int threadId) {
        if (debug) {
            System.out.println("Thread " + threadId + " computing element (" + row + ", " + col + ")");
        }

        int sum = 0;
        for (int k = 0; k < colsA; k++) {
            sum += matrixA[row][k] * matrixB[k][col];
        }
        result[row][col] = sum;
    }

    /**
     * Strategy 1: Each thread computes consecutive elements, going row after row.
     */
    public void multiplyRowWise(int numThreads) throws InterruptedException {
        Thread[] threads = new Thread[numThreads];
        int totalElements = rowsA * colsB;
        int elementsPerThread = totalElements / numThreads;

        for (int t = 0; t < numThreads; t++) {
            final int threadId = t;
            final int startElement = t * elementsPerThread;
            final int endElement = (t == numThreads - 1) ? totalElements : (t + 1) * elementsPerThread;

            threads[t] = new Thread(() -> {
                for (int elem = startElement; elem < endElement; elem++) {
                    int row = elem / colsB;
                    int col = elem % colsB;
                    computeElement(row, col, threadId);
                }
            });
            threads[t].start();
        }

        for (Thread thread : threads) {
            thread.join();
        }
    }

    /**
     * Strategy 2: Each thread computes consecutive elements, going column after
     * column.
     */
    public void multiplyColumnWise(int numThreads) throws InterruptedException {
        Thread[] threads = new Thread[numThreads];
        int totalElements = rowsA * colsB;
        int elementsPerThread = totalElements / numThreads;

        for (int t = 0; t < numThreads; t++) {
            final int threadId = t;
            final int startElement = t * elementsPerThread;
            final int endElement = (t == numThreads - 1) ? totalElements : (t + 1) * elementsPerThread;

            threads[t] = new Thread(() -> {
                for (int elem = startElement; elem < endElement; elem++) {
                    // Column-major order: traverse column by column
                    int col = elem / rowsA;
                    int row = elem % rowsA;
                    computeElement(row, col, threadId);
                }
            });
            threads[t].start();
        }

        for (Thread thread : threads) {
            thread.join();
        }
    }

    /**
     * Strategy 3: Each thread takes every k-th element (where k is the number of
     * threads),
     * going row by row.
     */
    public void multiplyKthElement(int numThreads) throws InterruptedException {
        Thread[] threads = new Thread[numThreads];
        int totalElements = rowsA * colsB;

        for (int t = 0; t < numThreads; t++) {
            final int threadId = t;

            threads[t] = new Thread(() -> {
                for (int elem = threadId; elem < totalElements; elem += numThreads) {
                    int row = elem / colsB;
                    int col = elem % colsB;
                    computeElement(row, col, threadId);
                }
            });
            threads[t].start();
        }

        for (Thread thread : threads) {
            thread.join();
        }
    }

    /**
     * Sequential multiplication for comparison. No threads are used.
     */
    public void multiplySequential() {
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < colsB; j++) {
                computeElement(i, j, 0);
            }
        }
    }

    public int[][] getResult() {
        return result;
    }

    /**
     * Helper method to print the matrix (for small matrices).
     */
    public void printResult() {
        System.out.println("Result Matrix:");
        for (int i = 0; i < rowsA; i++) {
            for (int j = 0; j < colsB; j++) {
                System.out.print(result[i][j] + "\t");
            }
            System.out.println();
        }
    }

    /**
     * Helper method to verify two matrices are equal.
     */
    public static boolean matricesEqual(int[][] m1, int[][] m2) {
        if (m1.length != m2.length || m1[0].length != m2[0].length) {
            return false;
        }
        for (int i = 0; i < m1.length; i++) {
            for (int j = 0; j < m1[0].length; j++) {
                if (m1[i][j] != m2[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
}
