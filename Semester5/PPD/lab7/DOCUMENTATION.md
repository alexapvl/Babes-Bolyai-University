# Lab 7: MPI Distributed Polynomial Multiplication

## Overview

This lab implements polynomial multiplication using MPI (Message Passing Interface) for distributed computing. We implement both the regular O(n²) algorithm and the Karatsuba O(n^1.585) algorithm, comparing their performance with the sequential CPU implementations from Lab 5.

**Bonus:** Big number multiplication is also implemented using the same approaches.

---

## 1. Algorithms

### 1.1 Regular Polynomial Multiplication (O(n²))

The regular algorithm multiplies two polynomials P(x) and Q(x) by computing all pairwise products of their coefficients.

**Mathematical Definition:**

Given:

- P(x) = p₀ + p₁x + p₂x² + ... + pₙx^n
- Q(x) = q₀ + q₁x + q₂x² + ... + qₘx^m

The product R(x) = P(x) × Q(x) has coefficients:

```
r_k = Σ(i+j=k) p_i × q_j
```

**Algorithm:**

```
for i = 0 to n:
    for j = 0 to m:
        result[i + j] += p[i] × q[j]
```

**Time Complexity:** O(n × m) ≈ O(n²) for equal-sized polynomials

### 1.2 Karatsuba Multiplication (O(n^1.585))

The Karatsuba algorithm reduces the number of recursive multiplications from 4 to 3 using a clever algebraic identity.

**Algorithm:**

Split polynomials at the midpoint:

- P(x) = P_high × x^m + P_low
- Q(x) = Q_high × x^m + Q_low

Then:

```
z₀ = P_low × Q_low
z₂ = P_high × Q_high
z₁ = (P_low + P_high) × (Q_low + Q_high) - z₀ - z₂
```

Result:

```
P(x) × Q(x) = z₂ × x^(2m) + z₁ × x^m + z₀
```

**Key Insight:** We compute 3 multiplications instead of 4:

- Traditional: P_low×Q_low, P_low×Q_high, P_high×Q_low, P_high×Q_high
- Karatsuba: z₀, z₂, (P_low+P_high)×(Q_low+Q_high)

**Time Complexity:** O(n^log₂3) ≈ O(n^1.585)

---

## 2. Distribution and Communication

### 2.1 MPI Architecture

The implementation uses a master-worker pattern:

```
┌─────────────────────────────────────────────────────────────────┐
│                         MASTER (Rank 0)                        │
│  • Generates polynomials                                       │
│  • Coordinates distribution                                    │
│  • Collects and combines results                               │
└───────────────────────┬─────────────────────────────────────────┘
                        │ MPI_Bcast (polynomials)
        ┌───────────────┼───────────────┬───────────────┐
        ▼               ▼               ▼               ▼
┌───────────────┐ ┌───────────────┐ ┌───────────────┐ ┌───────────────┐
│   Worker 0    │ │   Worker 1    │ │   Worker 2    │ │   Worker 3    │
│ (also Master) │ │               │ │               │ │               │
│  Computes     │ │  Computes     │ │  Computes     │ │  Computes     │
│  chunk [0,k)  │ │  chunk [k,2k) │ │  chunk [2k,3k)│ │  chunk [3k,n) │
└───────┬───────┘ └───────┬───────┘ └───────┬───────┘ └───────┬───────┘
        │                 │                 │                 │
        └─────────────────┴─────────────────┴─────────────────┘
                                │
                        MPI_Reduce (SUM)
                                │
                                ▼
                    ┌───────────────────────┐
                    │   Final Result at     │
                    │       Master          │
                    └───────────────────────┘
```

### 2.2 Regular Algorithm Distribution

**Strategy:** Partition the first polynomial's coefficients among processes.

```java
// Each process handles a range of coefficients from p1
int chunkSize = (n1 + size - 1) / size;
int start = rank * chunkSize;
int end = Math.min(start + chunkSize, n1);

// Each process computes partial products
for (int i = start; i < end; i++) {
    for (int j = 0; j < n2; j++) {
        localResult[i + j] += p1[i] * p2[j];
    }
}
```

**Communication Pattern:**

1. `MPI_Bcast`: Master broadcasts both polynomials to all workers
2. Each worker computes local partial results
3. `MPI_Reduce`: All partial results are summed at master

**Message Complexity:**

- Broadcast: O(n) data, O(log p) messages
- Reduce: O(2n) data, O(log p) messages
- Total: O(n log p)

### 2.3 Karatsuba Algorithm Distribution

**Strategy:** Distribute the three recursive sub-problems to different process groups.

```
                    P(x) × Q(x)
                         │
           ┌─────────────┼─────────────┐
           ▼             ▼             ▼
    ┌────────────┐ ┌────────────┐ ┌────────────┐
    │    z₀     │ │    z₂     │ │    z₁     │
    │ P_low×Q_low│ │P_high×Q_high│ │(P_l+P_h)× │
    │           │ │           │ │(Q_l+Q_h)  │
    └─────┬──────┘ └─────┬──────┘ └─────┬──────┘
          │              │              │
    Ranks 0..k-1    Ranks k..2k-1   Ranks 2k..p-1
```

**Distribution Logic:**

```java
int groupSize = size / 3;
int group;

if (rank < groupSize) {
    group = 0;  // Compute z0
} else if (rank < 2 * groupSize) {
    group = 1;  // Compute z2
} else {
    group = 2;  // Compute z1
}
```

**Communication Pattern:**

1. `MPI_Bcast`: Polynomials broadcast to all
2. Point-to-point: Sub-problem results sent to master
3. Master combines: result = z₀ + z₁×x^m + z₂×x^(2m)

### 2.4 Load Balancing

The work is distributed evenly by:

1. **Coefficient partitioning**: Each process handles ⌈n/p⌉ coefficients
2. **Handling remainder**: Last process may have fewer coefficients
3. **Synchronization**: `MPI_Barrier` ensures consistent timing

---

## 3. Performance Measurements

### 3.1 Test Environment

| Parameter           | Value             |
| ------------------- | ----------------- |
| CPU                 | Apple Silicon Mac |
| Cores               | 10                |
| RAM                 | 16 GB             |
| Java Version        | JDK 21            |
| MPJ Express Version | 0.44              |
| OS                  | macOS             |

### 3.2 Polynomial Multiplication Results

#### Java CPU Implementation (Lab 5) - Degree 1000

| Algorithm                   | Time (ms) | Speedup | Notes                   |
| --------------------------- | --------- | ------- | ----------------------- |
| Regular Sequential          | 0.99      | 1.00x   | Baseline                |
| Regular Parallel (10 thr)   | 0.92      | 1.07x   | Slight improvement      |
| Karatsuba Sequential        | 1.17      | 0.84x   | Overhead at small sizes |
| Karatsuba Parallel (10 thr) | 4.64      | 0.21x   | ForkJoin overhead       |

#### MPI Implementation (Lab 7) - Various Degrees

| Degree | Sequential | MPI Regular (4p) | MPI Karatsuba (4p) | Best Speedup     |
| ------ | ---------- | ---------------- | ------------------ | ---------------- |
| 100    | 0.16 ms    | 1.08 ms          | 0.80 ms            | 0.15x (overhead) |
| 500    | 1.44 ms    | 1.75 ms          | 1.80 ms            | 0.82x (overhead) |
| 1000   | 2.46 ms    | 2.21 ms          | 1.53 ms            | **1.61x** ✓      |
| 2000   | 1.40 ms    | 3.32 ms          | 3.08 ms            | 0.42x (overhead) |
| 5000   | 8.87 ms    | 17.21 ms         | 3.53 ms            | **2.51x** ✓      |

#### Performance Analysis - Degree 5000 (Best Case)

| Algorithm                  | Time (ms) | Speedup vs Sequential |
| -------------------------- | --------- | --------------------- |
| Sequential Regular         | 8.87      | 1.00x (baseline)      |
| MPI Regular (4 proc)       | 17.21     | 0.52x (slower)        |
| **MPI Karatsuba (4 proc)** | **3.53**  | **2.51x** ✓           |

### 3.3 Key Observations

#### Crossover Point Analysis

| Problem Size | Winner        | Reason                         |
| ------------ | ------------- | ------------------------------ |
| n < 500      | Sequential    | MPI communication overhead     |
| n ≈ 1000     | MPI Karatsuba | Karatsuba benefits start       |
| n > 2000     | MPI Karatsuba | Clear advantage (2.5x speedup) |

#### Communication vs Computation Trade-off

- **Small problems (n < 500)**: Communication time > Computation time
- **Large problems (n > 1000)**: Karatsuba's O(n^1.585) wins over O(n²)
- **MPI Regular**: Always slower than sequential (parallelization overhead)
- **MPI Karatsuba**: Wins at larger sizes due to algorithmic advantage

### 3.4 Comparison: Java Threads vs MPI (Degree 1000)

| Implementation          | Time (ms) | Notes                      |
| ----------------------- | --------- | -------------------------- |
| Java Regular Sequential | 0.99      | Baseline                   |
| Java Regular Parallel   | 0.92      | 10 threads, shared memory  |
| MPI Regular (4 proc)    | 2.21      | Process isolation overhead |
| MPI Karatsuba (4 proc)  | 1.53      | Algorithm beats overhead   |

**Conclusion**: For single-machine execution, Java threads outperform MPI due to shared memory. MPI's advantage is scalability across multiple machines.

---

## 4. Analysis and Conclusions

### 4.1 Performance Observations

1. **MPI Overhead**: For small problem sizes (n < 500), MPI communication overhead exceeds computation benefits. The crossover point where MPI Karatsuba becomes beneficial is around **degree 1000**.

2. **Karatsuba vs Regular**: Karatsuba shows its asymptotic advantage at larger sizes. The theoretical O(n^1.585) vs O(n²) becomes noticeable when **n > 1000**, achieving **2.51x speedup** at degree 5000.

3. **MPI vs Java Threads**:

   - Java threads have lower communication overhead (shared memory)
   - MPI scales better across multiple machines
   - For single-machine parallelism, Java threads often win

4. **Parallel Efficiency**: Efficiency decreases with more processes due to:
   - Communication overhead
   - Load imbalance
   - Synchronization costs

### 4.2 When to Use Each Approach

| Scenario                            | Recommended Approach                  |
| ----------------------------------- | ------------------------------------- |
| Small polynomials (n < 100)         | Sequential Regular                    |
| Medium polynomials (100 < n < 1000) | Java Parallel or Sequential Karatsuba |
| Large polynomials (n > 1000)        | MPI Karatsuba                         |
| Distributed systems                 | MPI (any algorithm)                   |
| Single machine, many cores          | Java ForkJoin with Karatsuba          |

### 4.3 Communication Analysis

**Bandwidth Requirements:**

- Regular MPI: 2n integers broadcast + 2n integers reduced = O(n)
- Karatsuba MPI: Similar, plus point-to-point for sub-problems

**Latency Sensitivity:**

- Karatsuba is more latency-sensitive due to sequential dependency in combining results
- Regular algorithm has more parallelism but higher data volume

### 4.4 Conclusions

1. **MPI enables true distributed computing** across multiple machines, unlike thread-based parallelism.

2. **Karatsuba algorithm** provides better asymptotic complexity but has higher constants; it's beneficial for large polynomials.

3. **Communication overhead** is the main limiting factor for MPI performance on small problems.

4. **Hybrid approaches** (MPI + threads) could combine benefits of both for optimal performance.

5. **The choice of algorithm** depends on problem size, available resources, and whether distribution across machines is required.

---

## 5. How to Run

### Prerequisites

1. **Java JDK** (version 8 or higher)
2. **MPJ Express** (download from https://mpj-express.org/)

### Installation

```bash
# 1. Download and extract MPJ Express
wget https://sourceforge.net/projects/mpjexpress/files/releases/mpj-v0_44.tar.gz
tar -xzf mpj-v0_44.tar.gz

# 2. Set environment variables
export MPJ_HOME=/path/to/mpj
export PATH=$MPJ_HOME/bin:$PATH
```

### Compile

```bash
cd lab7

# Compile Java CPU implementation only
make compile

# Compile MPI implementation only
make compile-mpi

# Compile both CPU and MPI
make compile-all
```

### Run Benchmarks

```bash
# Run Java CPU benchmark
make benchmark-poly

# Run MPI polynomial benchmark (4 processes)
make mpi-poly NP=4

# Run MPI big number benchmark
make mpi-bignum NP=4

# Run comprehensive MPI benchmark
make mpi-benchmark NP=4

# Compare Java CPU vs MPI side-by-side
make compare NP=4

# Show all available commands
make help
```

---

## 6. File Structure

```
lab7/
├── Polynomial.java              # Polynomial class
├── RegularMultiplication.java   # O(n²) algorithms (sequential & parallel)
├── KaratsubaMultiplication.java # O(n^1.585) algorithms
├── BigNumber.java               # Big number class
├── BigNumberMultiplication.java # Big number multiplication
├── PerformanceBenchmark.java    # Java CPU benchmarks
├── BigNumberBenchmark.java      # Big number benchmarks
├── Main.java                    # Interactive menu
├── Makefile                     # Build automation (CPU + MPI targets)
├── DOCUMENTATION.md             # This file
│
└── mpi/
    ├── MPIPolynomial.java       # MPI polynomial multiplication
    ├── MPIBigNumber.java        # MPI big number multiplication
    ├── MPIBenchmark.java        # Comprehensive MPI benchmarks
    └── run_comparison.sh        # Comparison script
```

---

## 7. References

1. Karatsuba, A., & Ofman, Y. (1962). "Multiplication of Many-Digital Numbers by Automatic Computers"
2. MPJ Express Documentation: https://mpj-express.org/docs.html
3. MPI Specification: https://www.mpi-forum.org/docs/
4. Parallel Programming concepts from course lectures
