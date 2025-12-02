/**
 * Simple test class to verify correctness of polynomial multiplication algorithms.
 */
public class PolynomialTest {

    private static int testsPassed = 0;
    private static int testsFailed = 0;

    public static void main(String[] args) {
        System.out.println("Running Polynomial Multiplication Tests...\n");

        testBasicMultiplication();
        testSmallPolynomials();
        testIdentity();
        testZero();
        testAllAlgorithmsProduceSameResult();

        System.out.println("\n" + "=".repeat(80));
        System.out.println("Test Results: " + testsPassed + " passed, " + testsFailed + " failed");
        System.out.println("=".repeat(80));

        if (testsFailed == 0) {
            System.out.println("✓ All tests passed!");
        } else {
            System.out.println("✗ Some tests failed!");
            System.exit(1);
        }
    }

    private static void testBasicMultiplication() {
        System.out.println("Test 1: Basic Multiplication (x + 1) * (x + 1) = x^2 + 2x + 1");
        
        // (x + 1) * (x + 1) = x^2 + 2x + 1
        Polynomial p1 = new Polynomial(new int[]{1, 1}); // 1 + x
        Polynomial p2 = new Polynomial(new int[]{1, 1}); // 1 + x
        Polynomial expected = new Polynomial(new int[]{1, 2, 1}); // 1 + 2x + x^2

        Polynomial result = RegularMultiplication.multiplySequential(p1, p2);
        
        if (result.equals(expected)) {
            System.out.println("  ✓ PASSED");
            testsPassed++;
        } else {
            System.out.println("  ✗ FAILED");
            System.out.println("    Expected: " + expected);
            System.out.println("    Got:      " + result);
            testsFailed++;
        }
    }

    private static void testSmallPolynomials() {
        System.out.println("\nTest 2: Small Polynomials (2x + 3) * (x^2 + x + 1)");
        
        // (3 + 2x) * (1 + x + x^2) = 3 + 5x + 5x^2 + 2x^3
        Polynomial p1 = new Polynomial(new int[]{3, 2}); // 3 + 2x
        Polynomial p2 = new Polynomial(new int[]{1, 1, 1}); // 1 + x + x^2
        Polynomial expected = new Polynomial(new int[]{3, 5, 5, 2}); // 3 + 5x + 5x^2 + 2x^3

        Polynomial result = KaratsubaMultiplication.multiplySequential(p1, p2);
        
        if (result.equals(expected)) {
            System.out.println("  ✓ PASSED");
            testsPassed++;
        } else {
            System.out.println("  ✗ FAILED");
            System.out.println("    Expected: " + expected);
            System.out.println("    Got:      " + result);
            testsFailed++;
        }
    }

    private static void testIdentity() {
        System.out.println("\nTest 3: Identity Multiplication P(x) * 1 = P(x)");
        
        Polynomial p1 = new Polynomial(new int[]{1, 2, 3, 4}); // 1 + 2x + 3x^2 + 4x^3
        Polynomial identity = new Polynomial(new int[]{1}); // 1

        Polynomial result = RegularMultiplication.multiplySequential(p1, identity);
        
        if (result.equals(p1)) {
            System.out.println("  ✓ PASSED");
            testsPassed++;
        } else {
            System.out.println("  ✗ FAILED");
            System.out.println("    Expected: " + p1);
            System.out.println("    Got:      " + result);
            testsFailed++;
        }
    }

    private static void testZero() {
        System.out.println("\nTest 4: Zero Multiplication P(x) * 0 = 0");
        
        Polynomial p1 = new Polynomial(new int[]{1, 2, 3, 4});
        Polynomial zero = new Polynomial(new int[]{0});
        Polynomial expected = new Polynomial(new int[]{0});

        Polynomial result = KaratsubaMultiplication.multiplySequential(p1, zero);
        
        if (result.equals(expected)) {
            System.out.println("  ✓ PASSED");
            testsPassed++;
        } else {
            System.out.println("  ✗ FAILED");
            System.out.println("    Expected: " + expected);
            System.out.println("    Got:      " + result);
            testsFailed++;
        }
    }

    private static void testAllAlgorithmsProduceSameResult() {
        System.out.println("\nTest 5: All Algorithms Produce Same Result");
        
        Polynomial p1 = new Polynomial(50, 100);
        Polynomial p2 = new Polynomial(50, 100);
        int numThreads = 4;

        Polynomial result1 = RegularMultiplication.multiplySequential(p1, p2);
        Polynomial result2 = RegularMultiplication.multiplyParallelV2(p1, p2, numThreads);
        Polynomial result3 = KaratsubaMultiplication.multiplySequential(p1, p2);
        Polynomial result4 = KaratsubaMultiplication.multiplyParallel(p1, p2, numThreads);

        boolean allEqual = result1.equals(result2) && 
                          result1.equals(result3) && 
                          result1.equals(result4);

        if (allEqual) {
            System.out.println("  ✓ PASSED - All four algorithms produce identical results");
            testsPassed++;
        } else {
            System.out.println("  ✗ FAILED - Algorithms produce different results");
            if (!result1.equals(result2)) System.out.println("    Regular Sequential ≠ Regular Parallel");
            if (!result1.equals(result3)) System.out.println("    Regular Sequential ≠ Karatsuba Sequential");
            if (!result1.equals(result4)) System.out.println("    Regular Sequential ≠ Karatsuba Parallel");
            testsFailed++;
        }
    }
}

