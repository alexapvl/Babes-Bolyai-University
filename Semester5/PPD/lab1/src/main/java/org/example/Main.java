import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class Main {
  private static final int NUM_THREADS = 16;
  private static final int OPERATIONS_PER_THREAD = 100000;
  private static final int NUM_WAREHOUSES = 5;
  private static final int NUM_UNIQUE_PRODUCTS = 10;
  private static final int INITIAL_QUANTITY_PER_PRODUCT_PER_WAREHOUSE = 1000;

  public static void main(String[] args) {
    System.out.println("Starting simulation with " + NUM_THREADS + " threads.");

    Inventory inventory = new Inventory(NUM_WAREHOUSES, NUM_UNIQUE_PRODUCTS,
        INITIAL_QUANTITY_PER_PRODUCT_PER_WAREHOUSE);

    if (!inventory.checkInventory()) {
      System.err.println("Initial inventory check failed!");
      return;
    } else {
      System.out.println("Initial inventory check passed.");
    }

    long startTime = System.currentTimeMillis();

    ExecutorService executor = Executors.newFixedThreadPool(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++) {
      executor.submit(new MoveWorker(inventory, OPERATIONS_PER_THREAD));
    }

    executor.shutdown();
    try {
      if (!executor.awaitTermination(1, TimeUnit.MINUTES)) {
        System.err.println("Threads did not terminate in time.");
        executor.shutdownNow();
      }
    } catch (InterruptedException e) {
      System.err.println("Main thread interrupted.");
      executor.shutdownNow();
      Thread.currentThread().interrupt();
    }

    long endTime = System.currentTimeMillis();

    System.out.println("All threads finished. Total time: " + (endTime - startTime) + " ms.");

    if (inventory.checkInventory()) {
      System.out.println("Final inventory check passed.");
    } else {
      System.err.println("Final inventory check failed!");
    }
  }
}
