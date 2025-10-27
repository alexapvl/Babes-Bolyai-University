import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ProducerConsumerQueue {
  private final Queue<Integer> queue;
  private final int capacity;
  private final Lock lock = new ReentrantLock();
  private final Condition notFull = lock.newCondition();
  private final Condition notEmpty = lock.newCondition();

  public ProducerConsumerQueue(int capacity) {
    this.queue = new LinkedList<>();
    this.capacity = capacity;
  }

  public void put(int value) throws InterruptedException {
    lock.lock();
    try {
      while (queue.size() == capacity) {
        notFull.await();
      }
      queue.add(value);
      notEmpty.signal();
    } finally {
      lock.unlock();
    }
  }

  public int get() throws InterruptedException {
    lock.lock();
    try {
      while (queue.isEmpty()) {
        notEmpty.await();
      }
      int value = queue.remove();
      notFull.signal();
      return value;
    } finally {
      lock.unlock();
    }
  }
}
