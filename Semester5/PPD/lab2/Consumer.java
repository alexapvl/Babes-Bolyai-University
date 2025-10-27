public class Consumer implements Runnable {
  private final ProducerConsumerQueue queue;
  private final int vectorSize;
  private int sum = 0;

  public Consumer(ProducerConsumerQueue queue, int vectorSize) {
    this.queue = queue;
    this.vectorSize = vectorSize;
  }

  @Override
  public void run() {
    for (int i = 0; i < vectorSize; i++) {
      try {
        int product = queue.get();
        sum += product;
      } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
      }
    }
  }

  public int getSum() {
    return sum;
  }
}
