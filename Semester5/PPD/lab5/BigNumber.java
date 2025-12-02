import java.util.Arrays;
import java.util.Random;

/**
 * Represents a large integer as an array of digits.
 * Used for implementing big number multiplication algorithms.
 */
public class BigNumber {
    private int[] digits; // digits stored in reverse order (least significant first)
    private boolean isNegative;

    /**
     * Creates a BigNumber from an array of digits.
     * @param digits Array of digits (least significant first)
     */
    public BigNumber(int[] digits) {
        this(digits, false);
    }

    public BigNumber(int[] digits, boolean isNegative) {
        // Remove leading zeros
        int size = digits.length;
        while (size > 1 && digits[size - 1] == 0) {
            size--;
        }
        this.digits = Arrays.copyOf(digits, size);
        this.isNegative = isNegative && !isZero();
    }

    /**
     * Creates a BigNumber from a string representation.
     */
    public BigNumber(String number) {
        if (number == null || number.isEmpty()) {
            throw new IllegalArgumentException("Number string cannot be null or empty");
        }

        isNegative = number.charAt(0) == '-';
        int start = isNegative ? 1 : 0;
        
        // Parse digits in reverse order
        digits = new int[number.length() - start];
        for (int i = start; i < number.length(); i++) {
            char c = number.charAt(i);
            if (!Character.isDigit(c)) {
                throw new IllegalArgumentException("Invalid digit: " + c);
            }
            digits[number.length() - 1 - i] = c - '0';
        }

        // Remove leading zeros
        int size = digits.length;
        while (size > 1 && digits[size - 1] == 0) {
            size--;
        }
        digits = Arrays.copyOf(digits, size);
        
        if (isZero()) {
            isNegative = false;
        }
    }

    /**
     * Creates a random BigNumber with specified number of digits.
     */
    public BigNumber(int numDigits) {
        if (numDigits < 1) {
            throw new IllegalArgumentException("Number of digits must be at least 1");
        }
        
        Random rand = new Random();
        digits = new int[numDigits];
        
        // Ensure most significant digit is non-zero
        digits[numDigits - 1] = rand.nextInt(9) + 1;
        
        for (int i = 0; i < numDigits - 1; i++) {
            digits[i] = rand.nextInt(10);
        }
        
        isNegative = false;
    }

    public int length() {
        return digits.length;
    }

    public int getDigit(int index) {
        if (index < 0 || index >= digits.length) {
            return 0;
        }
        return digits[index];
    }

    public int[] getDigits() {
        return Arrays.copyOf(digits, digits.length);
    }

    public boolean isNegative() {
        return isNegative;
    }

    public boolean isZero() {
        return digits.length == 1 && digits[0] == 0;
    }

    /**
     * Adds two BigNumbers.
     */
    public static BigNumber add(BigNumber a, BigNumber b) {
        if (a.isNegative == b.isNegative) {
            return addAbsolute(a, b, a.isNegative);
        } else {
            // Different signs - perform subtraction
            int cmp = compareAbsolute(a, b);
            if (cmp == 0) {
                return new BigNumber(new int[]{0});
            } else if (cmp > 0) {
                return subtractAbsolute(a, b, a.isNegative);
            } else {
                return subtractAbsolute(b, a, b.isNegative);
            }
        }
    }

    /**
     * Subtracts b from a.
     */
    public static BigNumber subtract(BigNumber a, BigNumber b) {
        if (a.isNegative != b.isNegative) {
            return addAbsolute(a, b, a.isNegative);
        } else {
            int cmp = compareAbsolute(a, b);
            if (cmp == 0) {
                return new BigNumber(new int[]{0});
            } else if (cmp > 0) {
                return subtractAbsolute(a, b, a.isNegative);
            } else {
                return subtractAbsolute(b, a, !a.isNegative);
            }
        }
    }

    private static BigNumber addAbsolute(BigNumber a, BigNumber b, boolean negative) {
        int maxLen = Math.max(a.length(), b.length());
        int[] result = new int[maxLen + 1];
        int carry = 0;

        for (int i = 0; i < maxLen || carry > 0; i++) {
            int sum = carry;
            if (i < a.length()) sum += a.getDigit(i);
            if (i < b.length()) sum += b.getDigit(i);
            
            result[i] = sum % 10;
            carry = sum / 10;
        }

        return new BigNumber(result, negative);
    }

    private static BigNumber subtractAbsolute(BigNumber a, BigNumber b, boolean negative) {
        // Assumes |a| >= |b|
        int[] result = new int[a.length()];
        int borrow = 0;

        for (int i = 0; i < a.length(); i++) {
            int diff = a.getDigit(i) - b.getDigit(i) - borrow;
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result[i] = diff;
        }

        return new BigNumber(result, negative);
    }

    private static int compareAbsolute(BigNumber a, BigNumber b) {
        if (a.length() != b.length()) {
            return a.length() - b.length();
        }
        for (int i = a.length() - 1; i >= 0; i--) {
            if (a.getDigit(i) != b.getDigit(i)) {
                return a.getDigit(i) - b.getDigit(i);
            }
        }
        return 0;
    }

    /**
     * Shifts the number by n positions (multiplies by 10^n).
     */
    public BigNumber shift(int n) {
        if (isZero() || n == 0) return this;
        
        int[] result = new int[digits.length + n];
        System.arraycopy(digits, 0, result, n, digits.length);
        return new BigNumber(result, isNegative);
    }

    /**
     * Creates a BigNumber from a subrange of digits.
     */
    public BigNumber subNumber(int start, int end) {
        end = Math.min(end, digits.length);
        if (start >= end) {
            return new BigNumber(new int[]{0});
        }
        int[] result = Arrays.copyOfRange(digits, start, end);
        return new BigNumber(result, false);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (!(obj instanceof BigNumber)) return false;
        BigNumber other = (BigNumber) obj;
        return isNegative == other.isNegative && 
               Arrays.equals(digits, other.digits);
    }

    @Override
    public int hashCode() {
        return Arrays.hashCode(digits) * (isNegative ? -1 : 1);
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        if (isNegative) {
            sb.append('-');
        }
        for (int i = digits.length - 1; i >= 0; i--) {
            sb.append(digits[i]);
        }
        return sb.toString();
    }

    /**
     * Returns a compact string representation for large numbers.
     */
    public String toCompactString() {
        if (digits.length <= 20) {
            return toString();
        }
        
        StringBuilder sb = new StringBuilder();
        if (isNegative) sb.append('-');
        
        // Show first 10 and last 10 digits
        for (int i = Math.min(9, digits.length - 1); i >= 0 && i >= digits.length - 10; i--) {
            sb.append(digits[i]);
        }
        sb.append("...");
        for (int i = Math.min(9, digits.length - 1); i >= 0; i--) {
            sb.append(digits[i]);
        }
        sb.append(" (").append(digits.length).append(" digits)");
        
        return sb.toString();
    }
}

