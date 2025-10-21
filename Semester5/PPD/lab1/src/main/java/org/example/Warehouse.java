import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Warehouse {
  private final int id;
  private final Map<Integer, Integer> productQuantities;
  private final Lock lock = new ReentrantLock();

  public Warehouse(int id, int numProducts, int initialQuantity) {
    this.id = id;
    this.productQuantities = new ConcurrentHashMap<>();
    for (int i = 0; i < numProducts; i++) {
      productQuantities.put(i, initialQuantity);
    }
  }

  public int getId() {
    return id;
  }

  public Lock getLock() {
    return lock;
  }

  public int getQuantity(int productId) {
    return productQuantities.getOrDefault(productId, 0);
  }

  public void addQuantity(int productId, int amount) {
    productQuantities.merge(productId, amount, Integer::sum);
  }

  public boolean removeQuantity(int productId, int amount) {
    if (getQuantity(productId) >= amount) {
      productQuantities.merge(productId, -amount, Integer::sum);
      return true;
    }
    return false;
  }
}
