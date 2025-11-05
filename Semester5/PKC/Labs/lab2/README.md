# Euler's Totient Function (φ function)

## Table of Contents

- [Introduction](#introduction)
- [Definition](#definition)
- [Mathematical Properties](#mathematical-properties)
- [Computing the Totient Function](#computing-the-totient-function)
- [Examples](#examples)
- [The Inverse Problem](#the-inverse-problem)
- [Applications](#applications)
- [Implementation](#implementation)

---

## Introduction

Euler's totient function, also denoted as **φ(n)** (phi of n), is one of the most important functions in number theory. Named after the Swiss mathematician Leonhard Euler, this function plays a crucial role in various areas of mathematics, including cryptography, particularly in the RSA encryption algorithm.

## Definition

**Euler's totient function φ(n)** counts the number of positive integers up to n that are **relatively prime** (coprime) to n.

Two numbers are relatively prime (or coprime) if their greatest common divisor (GCD) is 1.

### Formal Definition

For a positive integer n:

```
φ(n) = |{k ∈ ℕ : 1 ≤ k ≤ n and gcd(k, n) = 1}|
```

In other words, φ(n) counts how many numbers from 1 to n share no common factors with n (except 1).

### Special Cases

- **φ(1) = 1** (by convention)
- **φ(p) = p - 1** for any prime p (all numbers less than a prime are coprime to it)

---

## Mathematical Properties

Euler's totient function has several important properties:

### 1. **For Prime Numbers**

If p is a prime number:

```
φ(p) = p - 1
```

**Example:** φ(7) = 6, because 1, 2, 3, 4, 5, 6 are all coprime to 7.

### 2. **For Prime Powers**

If p is prime and k ≥ 1:

```
φ(p^k) = p^k - p^(k-1) = p^(k-1) × (p - 1)
```

**Example:** φ(9) = φ(3²) = 3² - 3¹ = 9 - 3 = 6

### 3. **Multiplicative Property**

Euler's function is **multiplicative**: if gcd(m, n) = 1, then:

```
φ(m × n) = φ(m) × φ(n)
```

**Example:** φ(15) = φ(3 × 5) = φ(3) × φ(5) = 2 × 4 = 8

### 4. **General Formula**

For any positive integer n with prime factorization:

```
n = p₁^k₁ × p₂^k₂ × ... × pᵣ^kᵣ
```

The totient function is:

```
φ(n) = n × (1 - 1/p₁) × (1 - 1/p₂) × ... × (1 - 1/pᵣ)
```

Or equivalently:

```
φ(n) = n × ∏(1 - 1/p) for all prime divisors p of n
```

### 5. **Sum of Divisors Property**

For any positive integer n:

```
∑ φ(d) = n
d|n
```

Where the sum is over all positive divisors d of n.

**Example:** For n = 6, divisors are 1, 2, 3, 6

```
φ(1) + φ(2) + φ(3) + φ(6) = 1 + 1 + 2 + 2 = 6 ✓
```

---

## Computing the Totient Function

There are two main approaches to computing φ(n):

### Method 1: Naive Counting Method

Count all numbers from 1 to n that are coprime to n:

```
φ(n) = count of {k : 1 ≤ k ≤ n, gcd(k, n) = 1}
```

**Time Complexity:** O(n log n)

**Advantage:** Simple to understand

**Disadvantage:** Slow for large n

### Method 2: Prime Factorization Method

1. Find the prime factorization of n
2. Apply the formula: φ(n) = n × ∏(1 - 1/p)

**Time Complexity:** O(√n)

**Advantage:** Much faster for large n

**Disadvantage:** Requires prime factorization

### Algorithm (Prime Factorization Method)

```
Algorithm: ComputeEulerTotient(n)
Input: A positive integer n
Output: φ(n)

1. If n = 1, return 1
2. result ← n
3. temp ← n
4. For each prime p dividing n:
   a. While temp is divisible by p:
      - temp ← temp / p
   b. result ← result × (p - 1) / p
5. Return result
```

---

## Examples

### Example 1: Small Numbers

| n   | Prime Factorization | φ(n) | Numbers coprime to n |
| --- | ------------------- | ---- | -------------------- |
| 1   | 1                   | 1    | {1}                  |
| 2   | 2                   | 1    | {1}                  |
| 3   | 3                   | 2    | {1, 2}               |
| 4   | 2²                  | 2    | {1, 3}               |
| 5   | 5                   | 4    | {1, 2, 3, 4}         |
| 6   | 2 × 3               | 2    | {1, 5}               |
| 7   | 7                   | 6    | {1, 2, 3, 4, 5, 6}   |
| 8   | 2³                  | 4    | {1, 3, 5, 7}         |
| 9   | 3²                  | 6    | {1, 2, 4, 5, 7, 8}   |
| 10  | 2 × 5               | 4    | {1, 3, 7, 9}         |

### Example 2: Computing φ(36)

```
36 = 2² × 3²

Method 1 (Formula):
φ(36) = 36 × (1 - 1/2) × (1 - 1/3)
      = 36 × (1/2) × (2/3)
      = 36 × 2/6
      = 12

Method 2 (Multiplicative):
φ(36) = φ(4) × φ(9)
      = (4 - 2) × (9 - 3)
      = 2 × 6
      = 12

Method 3 (Counting):
Numbers from 1-36 coprime to 36:
{1, 5, 7, 11, 13, 17, 19, 23, 25, 29, 31, 35}
Count = 12 ✓
```

### Example 3: Computing φ(100)

```
100 = 2² × 5²

φ(100) = 100 × (1 - 1/2) × (1 - 1/5)
       = 100 × (1/2) × (4/5)
       = 100 × 4/10
       = 40
```

---

## The Inverse Problem

Given a value v and a bound b, find all natural numbers n < b such that φ(n) = v.

### Problem Statement

**Input:**

- v: target Euler function value
- b: upper bound

**Output:**

- List of all n < b where φ(n) = v

### Algorithm

```
Algorithm: FindNumbersWithEulerValue(v, b)
Input: Target value v, bound b
Output: List of all n < b where φ(n) = v

1. result ← empty list
2. For n from 1 to b-1:
   a. If φ(n) = v:
      - Add n to result
3. Return result
```

### Interesting Observations

1. **φ(n) = 1** has solutions: n ∈ {1, 2}

2. **φ(n) = 2** has solutions: n ∈ {3, 4, 6}

   - 3 is prime: φ(3) = 2
   - 4 = 2²: φ(4) = 2
   - 6 = 2×3: φ(6) = 2

3. **Odd values:** If φ(n) is odd, then n must be 1 or 2 (the only cases where φ is odd)

4. **Even values:** For v even and v ≥ 2, there can be multiple solutions

5. **No solution:** Not every positive integer is a value of φ(n). For example:
   - There is no n such that φ(n) = 3, 5, 7, 9, 11, ...

### Example: Finding all n < 30 where φ(n) = 6

```
φ(7) = 6   (7 is prime)
φ(9) = 6   (9 = 3², φ(9) = 9 - 3 = 6)
φ(14) = 6  (14 = 2×7, φ(14) = φ(2)×φ(7) = 1×6 = 6)
φ(18) = 6  (18 = 2×3², φ(18) = φ(2)×φ(9) = 1×6 = 6)

Answer: {7, 9, 14, 18}
```

---

## Applications

### 1. **Number Theory**

- **Fermat's Little Theorem:** If p is prime and gcd(a, p) = 1, then a^(p-1) ≡ 1 (mod p)
- **Euler's Theorem:** If gcd(a, n) = 1, then a^φ(n) ≡ 1 (mod n)

### 2. **Cryptography (RSA Algorithm)**

The RSA encryption algorithm relies heavily on Euler's totient function:

- Choose two large primes p and q
- Compute n = p × q
- Compute φ(n) = (p - 1)(q - 1)
- Choose encryption exponent e where gcd(e, φ(n)) = 1
- Compute decryption exponent d where d × e ≡ 1 (mod φ(n))

### 3. **Primitive Roots**

The number of primitive roots modulo n (if they exist) is φ(φ(n)).

### 4. **Cyclic Groups**

In group theory, the order of the multiplicative group of integers modulo n is φ(n).

### 5. **Generating Functions**

Used in combinatorics and the study of partitions.

---

## Implementation

### Project Structure

This implementation includes:

1. **`euler_function.py`** - Main implementation file containing:

   - `gcd(a, b)` - Compute greatest common divisor
   - `euler_function_naive(n)` - Naive counting method
   - `prime_factorization(n)` - Find prime factors
   - `euler_function(n)` - Efficient computation using prime factorization
   - `find_numbers_with_euler_value(v, b)` - Solve the inverse problem
   - `display_euler_values(start, end)` - Display φ(n) for a range

2. **`example_usage.py`** - Demonstration and interactive usage

### Usage Examples

#### Computing φ(n) for a specific number

```python
from euler_function import euler_function

n = 36
result = euler_function(n)
print(f"φ({n}) = {result}")  # Output: φ(36) = 12
```

#### Finding all numbers with φ(n) = v

```python
from euler_function import find_numbers_with_euler_value

v = 6
b = 30
numbers = find_numbers_with_euler_value(v, b)
print(f"Numbers n < {b} where φ(n) = {v}: {numbers}")
# Output: Numbers n < 30 where φ(n) = 6: [7, 9, 14, 18]
```

#### Running the main program

```bash
python euler_function.py
```

This will:

1. Display φ(n) for various numbers
2. Prompt for interactive input to solve the inverse problem
3. Show interesting cases and properties

#### Running examples

```bash
python example_usage.py
```

This will:

1. Show computed values for common numbers
2. Demonstrate the inverse problem with test cases
3. Illustrate mathematical properties
4. Enter interactive mode for custom queries

---

## Complexity Analysis

### Time Complexity

| Operation                       | Naive Method   | Optimized Method |
| ------------------------------- | -------------- | ---------------- |
| Computing φ(n)                  | O(n log n)     | O(√n)            |
| Prime factorization             | -              | O(√n)            |
| Finding all n < b with φ(n) = v | O(b × n log n) | O(b × √n)        |

### Space Complexity

- O(k) where k is the number of distinct prime factors of n (typically small)

---

## Further Reading

- **Euler's Theorem and Applications**
- **RSA Cryptography**
- **Multiplicative Functions in Number Theory**
- **Primitive Roots and Discrete Logarithms**
- **Dirichlet's Theorem**

---

## References

1. Hardy, G. H., & Wright, E. M. (2008). _An Introduction to the Theory of Numbers_. Oxford University Press.
2. Burton, D. M. (2010). _Elementary Number Theory_. McGraw-Hill Education.
3. Rosen, K. H. (2012). _Elementary Number Theory and Its Applications_. Pearson.
4. Ireland, K., & Rosen, M. (1990). _A Classical Introduction to Modern Number Theory_. Springer.

---

## License

This implementation is provided for educational purposes.

## Author

PKC Lab 2 - Euler's Totient Function Implementation
