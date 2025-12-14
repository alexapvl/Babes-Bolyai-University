import java.util.Scanner;

/**
 * Main entry point for the polynomial and big number multiplication project.
 * Provides an interactive menu to run tests, benchmarks, and demonstrations.
 */
public class Main {

  public static void main(String[] args) {
    if (args.length > 0) {
      handleCommandLineArgs(args);
    } else {
      showInteractiveMenu();
    }
  }

  private static void handleCommandLineArgs(String[] args) {
    String command = args[0].toLowerCase();

    switch (command) {
      case "test":
        System.out.println("Running Polynomial Tests...\n");
        PolynomialTest.main(new String[] {});
        break;

      case "benchmark-poly":
        if (args.length > 1) {
          int degree = Integer.parseInt(args[1]);
          int threads = args.length > 2 ? Integer.parseInt(args[2]) : Runtime.getRuntime().availableProcessors();
          PerformanceBenchmark.main(new String[] { String.valueOf(degree), String.valueOf(threads) });
        } else {
          PerformanceBenchmark.main(new String[] {});
        }
        break;

      case "benchmark-bignum":
        if (args.length > 1) {
          int digits = Integer.parseInt(args[1]);
          int threads = args.length > 2 ? Integer.parseInt(args[2]) : Runtime.getRuntime().availableProcessors();
          BigNumberBenchmark.main(new String[] { String.valueOf(digits), String.valueOf(threads) });
        } else {
          BigNumberBenchmark.main(new String[] {});
        }
        break;

      case "demo":
        runDemo();
        break;

      default:
        printUsage();
    }
  }

  private static void showInteractiveMenu() {
    Scanner scanner = new Scanner(System.in);
    boolean running = true;

    while (running) {
      System.out.println("\n" + "═".repeat(80));
      System.out.println("        POLYNOMIAL & BIG NUMBER MULTIPLICATION - LAB 5");
      System.out.println("═".repeat(80));
      System.out.println("\n  1. Run Polynomial Tests");
      System.out.println("  2. Run Quick Polynomial Benchmark");
      System.out.println("  3. Run Comprehensive Polynomial Benchmark");
      System.out.println("  4. Run Big Number Benchmark");
      System.out.println("  5. Run Demo (Small Example)");
      System.out.println("  6. Custom Polynomial Benchmark");
      System.out.println("  7. Custom Big Number Benchmark");
      System.out.println("  0. Exit");
      System.out.println("\n" + "─".repeat(80));
      System.out.print("Enter your choice: ");

      try {
        int choice = scanner.nextInt();
        scanner.nextLine(); // Consume newline

        switch (choice) {
          case 1:
            PolynomialTest.main(new String[] {});
            break;

          case 2:
            System.out.println("\nRunning quick benchmark (degree 1000)...");
            PerformanceBenchmark.main(new String[] { "1000" });
            break;

          case 3:
            System.out.println("\nRunning comprehensive benchmark...");
            PerformanceBenchmark.main(new String[] {});
            break;

          case 4:
            System.out.println("\nRunning big number benchmark (1000 digits)...");
            BigNumberBenchmark.main(new String[] { "1000" });
            break;

          case 5:
            runDemo();
            break;

          case 6:
            System.out.print("Enter polynomial degree: ");
            int degree = scanner.nextInt();
            System.out.print("Enter number of threads (0 for default): ");
            int threads = scanner.nextInt();
            if (threads == 0) {
              threads = Runtime.getRuntime().availableProcessors();
            }
            PerformanceBenchmark.main(new String[] {
                String.valueOf(degree),
                String.valueOf(threads)
            });
            break;

          case 7:
            System.out.print("Enter number of digits: ");
            int digits = scanner.nextInt();
            System.out.print("Enter number of threads (0 for default): ");
            int threadsNum = scanner.nextInt();
            if (threadsNum == 0) {
              threadsNum = Runtime.getRuntime().availableProcessors();
            }
            BigNumberBenchmark.main(new String[] {
                String.valueOf(digits),
                String.valueOf(threadsNum)
            });
            break;

          case 0:
            System.out.println("\nExiting... Goodbye!");
            running = false;
            break;

          default:
            System.out.println("\nInvalid choice. Please try again.");
        }

        if (running && choice != 0) {
          System.out.println("\nPress Enter to continue...");
          scanner.nextLine();
        }

      } catch (Exception e) {
        System.out.println("\nError: Invalid input. Please enter a number.");
        scanner.nextLine(); // Clear buffer
      }
    }

    scanner.close();
  }

  private static void runDemo() {
    System.out.println("\n" + "═".repeat(80));
    System.out.println("                           DEMONSTRATION");
    System.out.println("═".repeat(80));

    // Demo 1: Simple polynomial multiplication
    System.out.println("\n【 Demo 1: Simple Polynomial Multiplication 】");
    System.out.println("─".repeat(80));

    Polynomial p1 = new Polynomial(new int[] { 1, 1 }); // 1 + x
    Polynomial p2 = new Polynomial(new int[] { 1, 1 }); // 1 + x

    System.out.println("P1(x) = " + p1);
    System.out.println("P2(x) = " + p2);
    System.out.println();

    Polynomial result1 = RegularMultiplication.multiplySequential(p1, p2);
    System.out.println("Using Regular Sequential:");
    System.out.println("Result = " + result1);
    System.out.println();

    Polynomial result2 = KaratsubaMultiplication.multiplySequential(p1, p2);
    System.out.println("Using Karatsuba Sequential:");
    System.out.println("Result = " + result2);
    System.out.println();

    System.out.println("Both methods produce the same result: " + result1.equals(result2));

    // Demo 2: Larger polynomial
    System.out.println("\n【 Demo 2: Medium Polynomial (Degree 100) 】");
    System.out.println("─".repeat(80));

    Polynomial p3 = new Polynomial(100, 50);
    Polynomial p4 = new Polynomial(100, 50);

    System.out.println("P3(x) = " + p3.toCompactString());
    System.out.println("P4(x) = " + p4.toCompactString());
    System.out.println();

    long start = System.nanoTime();
    Polynomial result3 = RegularMultiplication.multiplySequential(p3, p4);
    long end = System.nanoTime();
    System.out.printf("Regular Sequential: %.2f ms%n", (end - start) / 1_000_000.0);

    start = System.nanoTime();
    Polynomial result4 = RegularMultiplication.multiplyParallelV2(p3, p4, 4);
    end = System.nanoTime();
    System.out.printf("Regular Parallel:   %.2f ms%n", (end - start) / 1_000_000.0);

    start = System.nanoTime();
    Polynomial result5 = KaratsubaMultiplication.multiplySequential(p3, p4);
    end = System.nanoTime();
    System.out.printf("Karatsuba Sequential: %.2f ms%n", (end - start) / 1_000_000.0);

    start = System.nanoTime();
    Polynomial result6 = KaratsubaMultiplication.multiplyParallel(p3, p4, 4);
    end = System.nanoTime();
    System.out.printf("Karatsuba Parallel:   %.2f ms%n", (end - start) / 1_000_000.0);

    System.out.println();
    System.out.println("Result: " + result3.toCompactString());
    System.out.println("All methods agree: " +
        (result3.equals(result4) && result3.equals(result5) && result3.equals(result6)));

    // Demo 3: Big number multiplication
    System.out.println("\n【 Demo 3: Big Number Multiplication 】");
    System.out.println("─".repeat(80));

    BigNumber n1 = new BigNumber("12345");
    BigNumber n2 = new BigNumber("6789");

    System.out.println("N1 = " + n1);
    System.out.println("N2 = " + n2);
    System.out.println();

    BigNumber result7 = BigNumberMultiplication.multiplySequential(n1, n2);
    System.out.println("Regular Sequential: " + result7);

    BigNumber result8 = BigNumberMultiplication.multiplyKaratsubaSequential(n1, n2);
    System.out.println("Karatsuba Sequential: " + result8);

    System.out.println();
    System.out.println("Verification: 12345 × 6789 = " + (12345L * 6789L));
    System.out.println("Both methods produce correct result: " + result7.equals(result8));

    System.out.println("\n" + "═".repeat(80));
    System.out.println("                      DEMONSTRATION COMPLETE");
    System.out.println("═".repeat(80));
  }

  private static void printUsage() {
    System.out.println("Usage: java Main [command] [options]");
    System.out.println();
    System.out.println("Commands:");
    System.out.println("  test                           Run polynomial tests");
    System.out.println("  benchmark-poly [degree] [threads]   Run polynomial benchmark");
    System.out.println("  benchmark-bignum [digits] [threads] Run big number benchmark");
    System.out.println("  demo                           Run demonstration");
    System.out.println();
    System.out.println("If no command is provided, interactive menu will be shown.");
    System.out.println();
    System.out.println("Examples:");
    System.out.println("  java Main test");
    System.out.println("  java Main benchmark-poly 1000 8");
    System.out.println("  java Main benchmark-bignum 1000");
    System.out.println("  java Main demo");
  }
}
