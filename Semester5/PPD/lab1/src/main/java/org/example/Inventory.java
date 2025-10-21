import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

public class Inventory {
  private final List<Warehouse> warehouses;
  private final Map<Integer, Integer> initialTotalQuantities;
  private final int numProducts;

  public Inventory(int numWarehouses, int numProducts, int initialQuantity) {
    this.numProducts = numProducts;
    this.warehouses = new ArrayList<>();
    for (int i = 0; i < numWarehouses; i++) {
      warehouses.add(new Warehouse(i, numProducts, initialQuantity));
    }
    this.initialTotalQuantities = calculateTotalQuantities();
  }

  private Map<Integer, Integer> calculateTotalQuantities() {
    Map<Integer, Integer> totalQuantities = new HashMap<>();
    for (int i = 0; i < numProducts; i++) {
      int total = 0;
      for (Warehouse warehouse : warehouses) {
        total += warehouse.getQuantity(i);
      }
      totalQuantities.put(i, total);
    }
    return totalQuantities;
  }

  public void move(int productId, int amount, int fromWarehouseId, int toWarehouseId) {
    if (fromWarehouseId == toWarehouseId) {
      return;
    }

    Warehouse w1 = warehouses.get(Math.min(fromWarehouseId, toWarehouseId));
    Warehouse w2 = warehouses.get(Math.max(fromWarehouseId, toWarehouseId));

    w1.getLock().lock();
    try {
      w2.getLock().lock();
      try {
        Warehouse fromWarehouse = warehouses.get(fromWarehouseId);
        Warehouse toWarehouse = warehouses.get(toWarehouseId);

        if (fromWarehouse.removeQuantity(productId, amount)) {
          toWarehouse.addQuantity(productId, amount);
        }
      } finally {
        w2.getLock().unlock();
      }
    } finally {
      w1.getLock().unlock();
    }
  }

  public boolean checkInventory() {
    List<Warehouse> sortedWarehouses = warehouses.stream()
        .sorted((w1, w2) -> Integer.compare(w1.getId(), w2.getId()))
        .collect(Collectors.toList());

    for (Warehouse w : sortedWarehouses) {
      w.getLock().lock();
    }

    boolean success = true;
    try {
      Map<Integer, Integer> currentTotals = calculateTotalQuantities();
      for (int i = 0; i < numProducts; i++) {
        if (!initialTotalQuantities.get(i).equals(currentTotals.get(i))) {
          System.err.println("Inventory check failed for product " + i +
              ": expected " + initialTotalQuantities.get(i) +
              ", got " + currentTotals.get(i));
          success = false;
        }
      }
    } finally {
      for (Warehouse w : sortedWarehouses) {
        w.getLock().unlock();
      }
    }
    return success;
  }

  public int getNumWarehouses() {
    return warehouses.size();
  }

  public int getNumProducts() {
    return numProducts;
  }
}
