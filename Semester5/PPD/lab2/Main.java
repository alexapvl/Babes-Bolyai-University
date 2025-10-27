import java.util.Random;
import java.util.Arrays;

public class Main {
  public static void main(String[] args) {
    if (args.length != 2) {
      System.err.println("Usage: java Main <vector_size> <queue_size>");
      System.exit(1);
    }

    int vectorSize = Integer.parseInt(args[0]);
    int queueSize = Integer.parseInt(args[1]);

    int[] vector1 = new int[vectorSize];
    int[] vector2 = new int[vectorSize];
    Random random = new Random();

    for (int i = 0; i < vectorSize; i++) {
      vector1[i] = random.nextInt(10);
      vector2[i] = random.nextInt(10);
    }

    ProducerConsumerQueue queue = new ProducerConsumerQueue(queueSize);
    Producer producer = new Producer(queue, vector1, vector2);
    Consumer consumer = new Consumer(queue, vectorSize);

    Thread producerThread = new Thread(producer);
    Thread consumerThread = new Thread(consumer);

    long startTime = System.currentTimeMillis();

    producerThread.start();
    consumerThread.start();

    try {
      producerThread.join();
      consumerThread.join();
    } catch (InterruptedException e) {
      e.printStackTrace();
    }

    long endTime = System.currentTimeMillis();

    System.out.println("Vector 1: " + Arrays.toString(vector1));
    System.out.println("Vector 2: " + Arrays.toString(vector2));
    System.out.println("Scalar product: " + consumer.getSum());
    System.out.println("Execution time: " + (endTime - startTime) + " ms");
  }
}
