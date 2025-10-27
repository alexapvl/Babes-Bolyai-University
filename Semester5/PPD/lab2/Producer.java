public class Producer implements Runnable {
  private final ProducerConsumerQueue queue;
  private final int[] vector1;
  private final int[] vector2;

  public Producer(ProducerConsumerQueue queue, int[] vector1, int[] vector2) {
    this.queue = queue;
    this.vector1 = vector1;
    this.vector2 = vector2;
  }

  @Override
  public void run() {
    for (int i = 0; i < vector1.length; i++) {
      try {
        int product = vector1[i] * vector2[i];
        queue.put(product);
      } catch (InterruptedException e) {
        Thread.currentThread().interrupt();
      }
    }
  }
}
