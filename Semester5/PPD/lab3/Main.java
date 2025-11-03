public class Main {
    
    /**
     * Generates a random matrix with values between 0 and 9.
     */
    private static int[][] generateRandomMatrix(int rows, int cols) {
        int[][] matrix = new int[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                matrix[i][j] = (int) (Math.random() * 10);
            }
        }
        return matrix;
    }

    /**
     * Runs a single experiment and measures execution time.
     */
    private static long runExperiment(int[][] matrixA, int[][] matrixB, 
                                      String strategy, int numThreads, 
                                      boolean debug) throws InterruptedException {
        MatrixMultiplication mm = new MatrixMultiplication(matrixA, matrixB, debug);
        
        long startTime = System.nanoTime();
        
        switch (strategy) {
            case "sequential":
                mm.multiplySequential();
                break;
            case "row-wise":
                mm.multiplyRowWise(numThreads);
                break;
            case "column-wise":
                mm.multiplyColumnWise(numThreads);
                break;
            case "kth-element":
                mm.multiplyKthElement(numThreads);
                break;
            default:
                throw new IllegalArgumentException("Unknown strategy: " + strategy);
        }
        
        long endTime = System.nanoTime();
        long duration = endTime - startTime;
        
        return duration;
    }

    /**
     * Runs performance tests with different configurations.
     */
    private static void runPerformanceTests() {
        System.out.println("=" .repeat(80));
        System.out.println("PERFORMANCE TESTS");
        System.out.println("=" .repeat(80));
        
        int[] matrixSizes = {100, 200, 500, 1000};
        int[] threadCounts = {1, 2, 4, 8};
        String[] strategies = {"sequential", "row-wise", "column-wise", "kth-element"};
        
        for (int size : matrixSizes) {
            System.out.println("\n" + "-".repeat(80));
            System.out.println("Matrix Size: " + size + "x" + size);
            System.out.println("-".repeat(80));
            
            // Generate test matrices
            int[][] matrixA = generateRandomMatrix(size, size);
            int[][] matrixB = generateRandomMatrix(size, size);
            
            for (String strategy : strategies) {
                if (strategy.equals("sequential")) {
                    try {
                        long duration = runExperiment(matrixA, matrixB, strategy, 1, false);
                        System.out.printf("%-20s: %10.3f ms%n", 
                                        "Sequential", duration / 1_000_000.0);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                } else {
                    System.out.println("\n  " + strategy.toUpperCase() + ":");
                    for (int numThreads : threadCounts) {
                        try {
                            // Run multiple times and average
                            int runs = 3;
                            long totalDuration = 0;
                            
                            for (int run = 0; run < runs; run++) {
                                totalDuration += runExperiment(matrixA, matrixB, 
                                                              strategy, numThreads, false);
                            }
                            
                            long avgDuration = totalDuration / runs;
                            System.out.printf("    %2d threads: %10.3f ms%n", 
                                            numThreads, avgDuration / 1_000_000.0);
                            
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }
                }
            }
        }
    }

    /**
     * Demonstrates the debugging output for a small matrix.
     */
    private static void runDebugExample() {
        System.out.println("=" .repeat(80));
        System.out.println("DEBUG EXAMPLE - 9x9 Matrix with 4 Threads");
        System.out.println("=" .repeat(80));
        
        int size = 9;
        int numThreads = 4;
        
        int[][] matrixA = generateRandomMatrix(size, size);
        int[][] matrixB = generateRandomMatrix(size, size);
        
        String[] strategies = {"row-wise", "column-wise", "kth-element"};
        
        for (String strategy : strategies) {
            System.out.println("\n" + "-".repeat(80));
            System.out.println("Strategy: " + strategy.toUpperCase());
            System.out.println("-".repeat(80));
            
            try {
                runExperiment(matrixA, matrixB, strategy, numThreads, true);
                Thread.sleep(100); // Small delay for readability
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * Verifies that all strategies produce the same result.
     */
    private static void runCorrectnessTest() {
        System.out.println("=" .repeat(80));
        System.out.println("CORRECTNESS TEST");
        System.out.println("=" .repeat(80));
        
        int size = 50;
        int numThreads = 4;
        
        int[][] matrixA = generateRandomMatrix(size, size);
        int[][] matrixB = generateRandomMatrix(size, size);
        
        try {
            // Sequential (baseline)
            MatrixMultiplication mmSeq = new MatrixMultiplication(matrixA, matrixB, false);
            mmSeq.multiplySequential();
            int[][] resultSeq = mmSeq.getResult();
            
            // Row-wise
            MatrixMultiplication mmRow = new MatrixMultiplication(matrixA, matrixB, false);
            mmRow.multiplyRowWise(numThreads);
            int[][] resultRow = mmRow.getResult();
            
            // Column-wise
            MatrixMultiplication mmCol = new MatrixMultiplication(matrixA, matrixB, false);
            mmCol.multiplyColumnWise(numThreads);
            int[][] resultCol = mmCol.getResult();
            
            // K-th element
            MatrixMultiplication mmKth = new MatrixMultiplication(matrixA, matrixB, false);
            mmKth.multiplyKthElement(numThreads);
            int[][] resultKth = mmKth.getResult();
            
            // Verify results
            boolean rowCorrect = MatrixMultiplication.matricesEqual(resultSeq, resultRow);
            boolean colCorrect = MatrixMultiplication.matricesEqual(resultSeq, resultCol);
            boolean kthCorrect = MatrixMultiplication.matricesEqual(resultSeq, resultKth);
            
            System.out.println("Row-wise strategy:     " + (rowCorrect ? "✓ PASS" : "✗ FAIL"));
            System.out.println("Column-wise strategy:  " + (colCorrect ? "✓ PASS" : "✗ FAIL"));
            System.out.println("K-th element strategy: " + (kthCorrect ? "✓ PASS" : "✗ FAIL"));
            
            if (rowCorrect && colCorrect && kthCorrect) {
                System.out.println("\n✓ All strategies produce correct results!");
            } else {
                System.out.println("\n✗ Some strategies produced incorrect results!");
            }
            
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    /**
     * Demonstrates cache effects with different access patterns.
     */
    private static void runCacheEffectsAnalysis() {
        System.out.println("\n" + "=" .repeat(80));
        System.out.println("CACHE EFFECTS ANALYSIS");
        System.out.println("=" .repeat(80));
        System.out.println("Row-wise access (row-major order) typically benefits from better cache locality");
        System.out.println("Column-wise access may suffer from cache misses due to non-sequential memory access");
        System.out.println("K-th element access has poor cache locality as it jumps around the matrix");
        System.out.println("-".repeat(80));
        
        int size = 1000;
        int[][] matrixA = generateRandomMatrix(size, size);
        int[][] matrixB = generateRandomMatrix(size, size);
        
        String[] strategies = {"row-wise", "column-wise", "kth-element"};
        int numThreads = 4;
        
        for (String strategy : strategies) {
            try {
                long duration = runExperiment(matrixA, matrixB, strategy, numThreads, false);
                System.out.printf("%-20s: %10.3f ms%n", 
                                strategy.toUpperCase(), duration / 1_000_000.0);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        System.out.println("\n");
        System.out.println("╔══════════════════════════════════════════════════════════════════════════════╗");
        System.out.println("║        PARALLEL MATRIX MULTIPLICATION - PPD LAB 3                           ║");
        System.out.println("╚══════════════════════════════════════════════════════════════════════════════╝");
        
        // 1. Run correctness test first
        runCorrectnessTest();
        
        // 2. Show debug example with small matrix
        System.out.println("\n\n");
        runDebugExample();
        
        // 3. Run performance tests
        System.out.println("\n\n");
        runPerformanceTests();
        
        // 4. Analyze cache effects
        System.out.println("\n\n");
        runCacheEffectsAnalysis();
        
        System.out.println("\n" + "=" .repeat(80));
        System.out.println("ALL TESTS COMPLETED");
        System.out.println("=" .repeat(80) + "\n");
    }
}

