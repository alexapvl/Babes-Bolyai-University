import java.util.Random;

public class MoveWorker implements Runnable {
  private final Inventory inventory;
  private final int numOperations;
  private final Random random = new Random();

  public MoveWorker(Inventory inventory, int numOperations) {
    this.inventory = inventory;
    this.numOperations = numOperations;
  }

  @Override
  public void run() {
    for (int i = 0; i < numOperations; i++) {
      int numWarehouses = inventory.getNumWarehouses();
      int numProducts = inventory.getNumProducts();

      int fromWarehouseId = random.nextInt(numWarehouses);
      int toWarehouseId = random.nextInt(numWarehouses);
      while (fromWarehouseId == toWarehouseId) {
        toWarehouseId = random.nextInt(numWarehouses);
      }
      int productId = random.nextInt(numProducts);
      int amount = random.nextInt(10) + 1;

      inventory.move(productId, amount, fromWarehouseId, toWarehouseId);
    }
  }
}
