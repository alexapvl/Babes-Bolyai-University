import java.util.Arrays;
import java.util.Random;

/**
 * Represents a polynomial with integer coefficients.
 * Polynomial is stored as an array where index i represents the coefficient of
 * x^i.
 */
public class Polynomial {
  private int[] coefficients;

  /**
   * Creates a polynomial from an array of coefficients.
   * 
   * @param coefficients Array where index i is the coefficient of x^i
   */
  public Polynomial(int[] coefficients) {
    // Remove leading zeros
    int size = coefficients.length;
    while (size > 1 && coefficients[size - 1] == 0) {
      size--;
    }
    this.coefficients = Arrays.copyOf(coefficients, size);
  }

  /**
   * Creates a polynomial of given degree with random coefficients.
   * 
   * @param degree         The degree of the polynomial
   * @param maxCoefficient Maximum value for coefficients
   */
  public Polynomial(int degree, int maxCoefficient) {
    Random rand = new Random();
    this.coefficients = new int[degree + 1];
    for (int i = 0; i <= degree; i++) {
      this.coefficients[i] = rand.nextInt(maxCoefficient);
    }
    // Ensure leading coefficient is non-zero
    if (this.coefficients[degree] == 0) {
      this.coefficients[degree] = 1;
    }
  }

  /**
   * Returns the degree of the polynomial.
   */
  public int getDegree() {
    return coefficients.length - 1;
  }

  /**
   * Returns the coefficient at the given index.
   */
  public int getCoefficient(int index) {
    if (index < 0 || index >= coefficients.length) {
      return 0;
    }
    return coefficients[index];
  }

  /**
   * Returns a copy of the coefficients array.
   */
  public int[] getCoefficients() {
    return Arrays.copyOf(coefficients, coefficients.length);
  }

  /**
   * Returns the length of the coefficient array.
   */
  public int length() {
    return coefficients.length;
  }

  /**
   * Adds two polynomials.
   */
  public static Polynomial add(Polynomial p1, Polynomial p2) {
    int maxLen = Math.max(p1.length(), p2.length());
    int[] result = new int[maxLen];

    for (int i = 0; i < maxLen; i++) {
      result[i] = p1.getCoefficient(i) + p2.getCoefficient(i);
    }

    return new Polynomial(result);
  }

  /**
   * Subtracts p2 from p1.
   */
  public static Polynomial subtract(Polynomial p1, Polynomial p2) {
    int maxLen = Math.max(p1.length(), p2.length());
    int[] result = new int[maxLen];

    for (int i = 0; i < maxLen; i++) {
      result[i] = p1.getCoefficient(i) - p2.getCoefficient(i);
    }

    return new Polynomial(result);
  }

  /**
   * Shifts the polynomial by n positions (multiplies by x^n).
   */
  public Polynomial shift(int n) {
    int[] result = new int[coefficients.length + n];
    System.arraycopy(coefficients, 0, result, n, coefficients.length);
    return new Polynomial(result);
  }

  /**
   * Creates a polynomial from a subrange of coefficients.
   */
  public Polynomial subPolynomial(int start, int end) {
    end = Math.min(end, coefficients.length);
    if (start >= end) {
      return new Polynomial(new int[] { 0 });
    }
    int[] result = Arrays.copyOfRange(coefficients, start, end);
    return new Polynomial(result);
  }

  @Override
  public boolean equals(Object obj) {
    if (this == obj)
      return true;
    if (!(obj instanceof Polynomial))
      return false;
    Polynomial other = (Polynomial) obj;
    return Arrays.equals(this.coefficients, other.coefficients);
  }

  @Override
  public int hashCode() {
    return Arrays.hashCode(coefficients);
  }

  @Override
  public String toString() {
    if (coefficients.length == 0)
      return "0";

    StringBuilder sb = new StringBuilder();
    for (int i = coefficients.length - 1; i >= 0; i--) {
      if (coefficients[i] == 0)
        continue;

      if (sb.length() > 0 && coefficients[i] > 0) {
        sb.append(" + ");
      } else if (coefficients[i] < 0) {
        sb.append(" - ");
      }

      int coef = Math.abs(coefficients[i]);
      if (i == 0 || coef != 1) {
        sb.append(coef);
      }

      if (i > 0) {
        sb.append("x");
        if (i > 1) {
          sb.append("^").append(i);
        }
      }
    }

    return sb.length() > 0 ? sb.toString() : "0";
  }

  /**
   * Returns a compact string representation for large polynomials.
   */
  public String toCompactString() {
    if (coefficients.length <= 5) {
      return toString();
    }
    return String.format("Polynomial[degree=%d, coefficients=[%d, %d, ..., %d, %d]]",
        getDegree(),
        coefficients[0],
        coefficients[1],
        coefficients[coefficients.length - 2],
        coefficients[coefficients.length - 1]);
  }
}
