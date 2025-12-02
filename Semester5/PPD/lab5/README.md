# Lab 5: Parallel Polynomial and Big Number Multiplication

This project implements polynomial and big number multiplication algorithms using both sequential and parallel approaches, comparing the regular O(n²) algorithm with the more efficient Karatsuba O(n^1.585) algorithm.

## Table of Contents

- [Overview](#overview)
- [Algorithms](#algorithms)
- [Synchronization](#synchronization)
- [Project Structure](#project-structure)
- [Compilation and Execution](#compilation-and-execution)
- [Performance Results](#performance-results)
- [Conclusion](#conclusion)

## Overview

The project consists of two main parts:

1. **Polynomial Multiplication**: Multiplying polynomials with integer coefficients
2. **Big Number Multiplication** (Bonus): Multiplying arbitrarily large integers

For each part, we implement:

- **Regular Sequential Algorithm**: O(n²) time complexity
- **Regular Parallel Algorithm**: Parallelized version of the O(n²) algorithm
- **Karatsuba Sequential Algorithm**: O(n^1.585) time complexity
- **Karatsuba Parallel Algorithm**: Parallelized version of Karatsuba

## Algorithms

### 1. Regular Multiplication Algorithm

#### Description

The regular multiplication algorithm is the standard "grade-school" multiplication approach.

**For Polynomials:**
Given two polynomials P(x) and Q(x):

- P(x) = p₀ + p₁x + p₂x² + ... + pₙxⁿ
- Q(x) = q₀ + q₁x + q₂x² + ... + qₘxᵐ

The product R(x) = P(x) × Q(x) is computed as:

```
R[k] = Σ(i+j=k) P[i] × Q[j]
```

**Time Complexity:** O(n²) where n is the degree of the polynomials

**Space Complexity:** O(n)

#### Sequential Implementation

```java
for (int i = 0; i < n1; i++) {
    for (int j = 0; j < n2; j++) {
        result[i + j] += p1.getCoefficient(i) * p2.getCoefficient(j);
    }
}
```

#### Parallel Implementation

The parallelization strategy divides the work by result coefficients. Each thread computes a range of result coefficients independently:

```java
// Each thread computes coefficients from start to end
for (int k = start; k < end; k++) {
    int sum = 0;
    // Find all pairs (i,j) where i+j = k
    for (int i = 0; i < n1; i++) {
        int j = k - i;
        if (j >= 0 && j < n2) {
            sum += p1.getCoefficient(i) * p2.getCoefficient(j);
        }
    }
    result[k] = sum;
}
```

**Advantages:**

- No synchronization needed (each thread writes to different indices)
- Good load balancing
- Simple to understand and implement

### 2. Karatsuba Multiplication Algorithm

#### Description

Karatsuba is a divide-and-conquer algorithm that reduces the number of multiplications from 4 to 3 for each recursive step.

**Algorithm:**

For two polynomials P(x) and Q(x), split them at position m:

- P(x) = P_low + P_high × x^m
- Q(x) = Q_low + Q_high × x^m

The product is computed using only 3 recursive multiplications:

1. z₀ = P_low × Q_low
2. z₂ = P_high × Q_high
3. z₁ = (P_low + P_high) × (Q_low + Q_high) - z₀ - z₂

**Result:** P(x) × Q(x) = z₀ + z₁ × x^m + z₂ × x^(2m)

**Time Complexity:** O(n^(log₂3)) ≈ O(n^1.585)

**Space Complexity:** O(n log n) due to recursion

#### Sequential Implementation

```java
// Base case: use regular multiplication for small polynomials
if (size <= THRESHOLD) {
    return regularMultiplication(p1, p2);
}

// Split polynomials
int mid = size / 2;
Polynomial p1Low = p1.subPolynomial(0, mid);
Polynomial p1High = p1.subPolynomial(mid, size);
Polynomial p2Low = p2.subPolynomial(0, mid);
Polynomial p2High = p2.subPolynomial(mid, size);

// Three recursive multiplications
Polynomial z0 = multiplyKaratsuba(p1Low, p2Low);
Polynomial z2 = multiplyKaratsuba(p1High, p2High);
Polynomial z1 = multiplyKaratsuba(
    add(p1Low, p1High),
    add(p2Low, p2High)
);
z1 = subtract(subtract(z1, z0), z2);

// Combine results
return add(add(z0, shift(z1, mid)), shift(z2, 2*mid));
```

#### Parallel Implementation

The three recursive multiplications (z₀, z₁, z₂) are independent and can be executed in parallel:

```java
Future<Polynomial> futureZ0 = executor.submit(() ->
    multiplyKaratsubaParallel(p1Low, p2Low, executor, depth+1)
);
Future<Polynomial> futureZ2 = executor.submit(() ->
    multiplyKaratsubaParallel(p1High, p2High, executor, depth+1)
);
Future<Polynomial> futureZ1 = executor.submit(() ->
    multiplyKaratsubaParallel(
        add(p1Low, p1High),
        add(p2Low, p2High),
        executor,
        depth+1
    )
);

// Wait for all results and combine
Polynomial z0 = futureZ0.get();
Polynomial z2 = futureZ2.get();
Polynomial z1 = futureZ1.get();
```

**Parallelization Strategy:**

- Depth-limited parallelism to avoid excessive thread creation
- Uses thread pool with fixed size
- Maximum parallelization depth: 4 levels
- Below threshold size, switches to sequential execution

## Synchronization

### Regular Parallel Algorithm

**Strategy 1: Result Coefficient Parallelization (Used in Implementation)**

- Each thread computes a distinct range of result coefficients
- **No synchronization required** because threads write to non-overlapping indices
- Best performance for this algorithm

**Strategy 2: Input Coefficient Parallelization (Alternative)**

- Each thread processes a range of coefficients from the first polynomial
- Requires **synchronized access** to result array (different threads may write to same index)
- Uses Java's `synchronized` keyword on result array

```java
synchronized (result) {
    result[i + j] += p1.getCoefficient(i) * p2.getCoefficient(j);
}
```

### Karatsuba Parallel Algorithm

**Synchronization Mechanism:**

- Uses `ExecutorService` with thread pool
- `Future<T>` objects for managing asynchronous computations
- `.get()` method provides implicit synchronization (waiting for results)

**Key Points:**

- No explicit locks needed for data access
- Synchronization handled by Java's concurrent framework
- Thread pool manages thread lifecycle
- Depth control prevents thread explosion

**Depth Control:**

```java
final int MAX_PARALLEL_DEPTH = 4;
if (depth > MAX_PARALLEL_DEPTH) {
    return multiplySequential(p1, p2);  // Switch to sequential
}
```

### Big Number Multiplication

Similar synchronization strategies are used for big number multiplication:

- Regular parallel uses synchronized blocks for result array access
- Karatsuba parallel uses Future-based coordination

## Project Structure

```
lab5/
├── README.md                           # This documentation
├── Polynomial.java                     # Polynomial representation
├── RegularMultiplication.java          # Regular O(n²) algorithms
├── KaratsubaMultiplication.java        # Karatsuba O(n^1.585) algorithms
├── PerformanceBenchmark.java           # Polynomial benchmarking
├── PolynomialTest.java                 # Unit tests for polynomials
├── BigNumber.java                      # Big number representation (Bonus)
├── BigNumberMultiplication.java        # Big number algorithms (Bonus)
└── BigNumberBenchmark.java             # Big number benchmarking (Bonus)
```

### Key Classes

#### Polynomial.java

Represents a polynomial with integer coefficients. Provides operations:

- Addition and subtraction
- Coefficient access
- Shifting (multiplication by x^n)
- Sub-polynomial extraction

#### RegularMultiplication.java

Implements both sequential and parallel versions of the O(n²) algorithm.

#### KaratsubaMultiplication.java

Implements both sequential and parallel versions of Karatsuba algorithm.

#### PerformanceBenchmark.java

Comprehensive benchmarking framework that:

- Performs warmup runs
- Measures execution time over multiple runs
- Computes statistics (min, max, average)
- Verifies correctness
- Calculates speedup metrics

## Compilation and Execution

### Compilation

```bash
# Compile all Java files
javac *.java

# Or compile specific components
javac Polynomial.java RegularMultiplication.java KaratsubaMultiplication.java PerformanceBenchmark.java
```

### Running Tests

```bash
# Run polynomial correctness tests
java PolynomialTest
```

### Running Benchmarks

#### Polynomial Multiplication

```bash
# Run comprehensive benchmark (multiple sizes)
java PerformanceBenchmark

# Run benchmark for specific polynomial degree
java PerformanceBenchmark <degree> [numThreads]

# Examples:
java PerformanceBenchmark 1000        # Degree 1000, default threads
java PerformanceBenchmark 5000 8      # Degree 5000, 8 threads
```

#### Big Number Multiplication (Bonus)

```bash
# Run comprehensive benchmark
java BigNumberBenchmark

# Run benchmark for specific number of digits
java BigNumberBenchmark <digits> [numThreads]

# Examples:
java BigNumberBenchmark 1000          # 1000 digits, default threads
java BigNumberBenchmark 5000 8        # 5000 digits, 8 threads
```

## Performance Results

### Test Environment

- **CPU:** [Your CPU model and core count]
- **RAM:** [Your RAM amount]
- **Java Version:** [Your Java version]
- **OS:** [Your operating system]

### Polynomial Multiplication Results

#### Degree 100

```
Algorithm                      | Avg Time (ms) | Speedup
-----------------------------------------------------------
Regular Sequential            |     X.XX      |   1.00x
Regular Parallel              |     X.XX      |   X.XXx
Karatsuba Sequential          |     X.XX      |   X.XXx
Karatsuba Parallel            |     X.XX      |   X.XXx
```

#### Degree 1000

```
Algorithm                      | Avg Time (ms) | Speedup
-----------------------------------------------------------
Regular Sequential            |     X.XX      |   1.00x
Regular Parallel              |     X.XX      |   X.XXx
Karatsuba Sequential          |     X.XX      |   X.XXx
Karatsuba Parallel            |     X.XX      |   X.XXx
```

#### Degree 5000

```
Algorithm                      | Avg Time (ms) | Speedup
-----------------------------------------------------------
Regular Sequential            |     X.XX      |   1.00x
Regular Parallel              |     X.XX      |   X.XXx
Karatsuba Sequential          |     X.XX      |   X.XXx
Karatsuba Parallel            |     X.XX      |   X.XXx
```

### Expected Performance Characteristics

#### Regular Algorithm

- **Sequential:** Baseline O(n²) performance
- **Parallel:** Expected speedup of 2-4x depending on core count
- **Best for:** Small to medium polynomials (degree < 500)

#### Karatsuba Algorithm

- **Sequential:** Faster than regular for large polynomials due to O(n^1.585) complexity
- **Parallel:** Additional 2-3x speedup over sequential Karatsuba
- **Best for:** Large polynomials (degree > 1000)

#### Trade-offs

1. **Overhead:** Parallel versions have thread management overhead
2. **Crossover Point:** Karatsuba becomes faster than regular around degree 500-1000
3. **Memory:** Karatsuba uses more memory due to recursion
4. **Scalability:** Parallel versions scale with available CPU cores

### Big Number Multiplication Results (Bonus)

Similar performance characteristics apply to big number multiplication:

- Regular algorithm: O(n²) for n digits
- Karatsuba algorithm: O(n^1.585) for n digits
- Parallel versions provide additional speedup

Expected crossover point: around 500-1000 digits

## Conclusion

### Key Findings

1. **Algorithm Choice Matters:**

   - Regular algorithm is simpler and faster for small inputs
   - Karatsuba is significantly faster for large inputs
   - Crossover point depends on constant factors and implementation

2. **Parallelization Benefits:**

   - Provides 2-4x speedup on multi-core systems
   - Most effective for large problem sizes
   - Requires careful synchronization strategy

3. **Synchronization Strategy:**

   - Avoiding synchronization (via work partitioning) is fastest
   - Future-based coordination works well for divide-and-conquer
   - Thread pool management is crucial for performance

4. **Implementation Considerations:**
   - Depth-limited parallelization prevents thread explosion
   - Threshold-based algorithm switching optimizes performance
   - Warmup runs are essential for accurate benchmarking

### Practical Applications

1. **Cryptography:** Large number multiplication in RSA, elliptic curve operations
2. **Computer Algebra Systems:** Symbolic computation, polynomial arithmetic
3. **Signal Processing:** Fast convolution using polynomial multiplication
4. **Scientific Computing:** High-precision arithmetic

### Future Improvements

1. **Additional Algorithms:**

   - Toom-Cook multiplication (O(n^1.465))
   - FFT-based multiplication (O(n log n))
   - Schönhage-Strassen algorithm for very large numbers

2. **Optimizations:**

   - Cache-friendly memory access patterns
   - SIMD vectorization
   - GPU acceleration for massive parallelism

3. **Adaptive Strategies:**
   - Dynamic algorithm selection based on input size
   - Adaptive thread count based on system load
   - Work stealing for better load balancing

## References

1. Karatsuba, A., & Ofman, Y. (1962). "Multiplication of Multidigit Numbers on Automata"
2. Knuth, D. E. (1997). "The Art of Computer Programming, Volume 2: Seminumerical Algorithms"
3. Java Concurrency in Practice by Brian Goetz
4. Introduction to Parallel Computing by Ananth Grama et al.

## Author

[Your Name]  
Babes-Bolyai University  
Semester 5 - Parallel and Distributed Programming  
Lab 5

## License

This project is created for educational purposes as part of the PPD course.
