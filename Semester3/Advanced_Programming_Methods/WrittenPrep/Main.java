import java.util.Arrays;
import java.util.List;

public class Main {
  public static void main(String[] args) {
    System.out.println(Written30jan2025());
  }

  private static Integer Written30jan2025() {
    List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
    Integer result = numbers.stream()
        .filter(num -> num % 3 != 0 && num % 7 != 0)
        .map(num -> (num - 1) * 11)
        .reduce(0, (sum, num) -> sum = (sum + num) % 5);

    return result;
  }
}

class A {
  protected int f1;
  static int s1 = 0;

  public A(int a) {
    this.f1 = a * s1;
    s1 = s1 + 1;
  }

  static int getS() {
    return s1;
  }

  int getS1(int x) {
    return (x * getS());
  }
}

/*
 * how to run this code:
 * 1. open terminal
 * 2. go to the directory where this file is located
 * 3. run the following commands:
 * javac Main.java
 * 
 */