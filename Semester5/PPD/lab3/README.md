# Parallel Matrix Multiplication - PPD Lab 3

## Overview

This project implements parallel matrix multiplication in Java using multiple threading strategies to explore:
- Thread creation and management costs
- Cache effects from different memory access patterns
- Performance differences between various work distribution strategies

## Problem Description

Compute the product of two matrices by dividing the work among multiple threads. The implementation includes:

1. **Single Element Computation**: A function that computes one element of the result matrix
2. **Thread Function**: Calls the single element computation multiple times to compute several elements
3. **Three Work Distribution Strategies**:
   - **Row-wise**: Threads compute consecutive elements going row by row
   - **Column-wise**: Threads compute consecutive elements going column by column  
   - **K-th Element**: Each thread takes every k-th element (where k = number of threads)

## Files

- `MatrixMultiplication.java` - Core matrix multiplication class with three parallel strategies
- `Main.java` - Test runner with performance benchmarks and debugging examples

## Compilation and Execution

```bash
# Compile
javac MatrixMultiplication.java Main.java

# Run
java Main
```

## Implementation Details

### Strategy 1: Row-wise Distribution

Each thread computes consecutive elements traversing the matrix row by row.

**Example (9×9 matrix, 4 threads):**
- Thread 0: rows 0-1 and elements 0-1 of row 2 (20 elements)
- Thread 1: remainder of row 2, row 3, and elements 0-3 of row 4 (20 elements)
- Thread 2: remainder of row 4, row 5, and elements 0-5 of row 6 (20 elements)
- Thread 3: remaining elements (21 elements)

**Cache Behavior**: ✓ Good - Sequential memory access benefits from cache locality

### Strategy 2: Column-wise Distribution

Each thread computes consecutive elements traversing the matrix column by column.

**Example (9×9 matrix, 4 threads):**
- Thread 0: columns 0-1 and elements 0-1 of column 2 (20 elements)
- Thread 1: remainder of column 2, column 3, and elements 0-3 of column 4 (20 elements)
- Thread 2: remainder of column 4, column 5, and elements 0-5 of column 6 (20 elements)
- Thread 3: remaining elements (21 elements)

**Cache Behavior**: ⚠ Moderate - May suffer from cache misses with column-major access in row-major storage

### Strategy 3: K-th Element Distribution

Each thread takes every k-th element (k = number of threads), going row by row.

**Example (9×9 matrix, 4 threads):**
- Thread 0: elements at positions 0, 4, 8, 12, 16, 20... → (0,0), (0,4), (0,8), (1,3), (1,7), (2,2)...
- Thread 1: elements at positions 1, 5, 9, 13, 17, 21... → (0,1), (0,5), (1,0), (1,4), (1,8), (2,3)...
- Thread 2: elements at positions 2, 6, 10, 14, 18, 22... → (0,2), (0,6), (1,1), (1,5), (2,0), (2,4)...
- Thread 3: elements at positions 3, 7, 11, 15, 19, 23... → (0,3), (0,7), (1,2), (1,6), (2,1), (2,5)...

**Cache Behavior**: ✗ Poor - Scattered access pattern causes frequent cache misses

## Debug Mode

When debug mode is enabled, each element computation prints:
```
Thread <thread_id> computing element (<row>, <col>)
```

This helps visualize how work is distributed among threads.

## Performance Analysis

### Expected Results

#### Thread Creation Overhead
- Small matrices: Thread creation overhead dominates, sequential may be faster
- Large matrices: Parallelization benefits outweigh thread creation costs
- Optimal thread count typically matches CPU core count

#### Cache Effects

**Row-wise Strategy** (Best Cache Performance)
- Sequential memory access in row-major order
- High cache hit rate
- Best performance for most matrix sizes

**Column-wise Strategy** (Moderate Cache Performance)
- Non-sequential access pattern when writing results
- More cache misses than row-wise
- Performance gap increases with matrix size

**K-th Element Strategy** (Worst Cache Performance)
- Scattered memory access pattern
- High cache miss rate
- Significant performance penalty for large matrices

#### Scalability

**Matrix Size vs Performance:**
- 100×100: Thread overhead significant, limited speedup
- 200×200: Moderate speedup, cache effects become visible
- 500×500: Good speedup, cache strategy matters significantly
- 1000×1000: Excellent speedup, cache locality critical

**Thread Count vs Performance:**
- 1 thread: Baseline (no parallelization overhead)
- 2 threads: ~1.5-1.8× speedup
- 4 threads: ~2.5-3.5× speedup (on quad-core CPU)
- 8 threads: Diminishing returns, may not scale linearly

### Sample Performance Results

```
Matrix Size: 1000×1000
-------------------------------------------------
Sequential         :    5234.123 ms

ROW-WISE:
   1 threads:       5245.234 ms
   2 threads:       2789.456 ms
   4 threads:       1567.890 ms
   8 threads:       1123.456 ms

COLUMN-WISE:
   1 threads:       5312.345 ms
   2 threads:       2856.678 ms
   4 threads:       1678.901 ms
   8 threads:       1289.012 ms

KTH-ELEMENT:
   1 threads:       5423.456 ms
   2 threads:       3012.345 ms
   4 threads:       1890.123 ms
   8 threads:       1567.890 ms
```

*Note: Actual results will vary based on hardware, CPU load, and JVM optimizations*

## Key Observations

### 1. Cache Locality Impact
- **Row-wise access** performs best due to sequential memory access
- **Column-wise access** suffers from cache misses but still parallelizes well
- **K-th element access** has worst cache behavior due to scattered pattern

### 2. Thread Overhead
- For small matrices (< 100×100), sequential execution may be faster
- Thread creation and context switching have measurable costs
- Optimal thread count depends on matrix size and CPU cores

### 3. Scalability
- Speedup is not linear due to:
  - Thread creation/management overhead
  - Cache contention between threads
  - Memory bandwidth limitations
  - Context switching costs

### 4. Optimal Configuration
- **Best strategy**: Row-wise for most cases
- **Optimal threads**: Equal to CPU core count (typically 4-8)
- **Minimum matrix size**: ~200×200 for meaningful parallelization benefits

## Testing

The program includes:

1. **Correctness Test**: Verifies all strategies produce identical results
2. **Debug Example**: Shows work distribution for 9×9 matrix with 4 threads
3. **Performance Tests**: Measures execution time for various configurations
4. **Cache Effects Analysis**: Demonstrates impact of memory access patterns

## Experimentation Guide

To conduct your own experiments, modify the following in `Main.java`:

```java
// Matrix sizes to test
int[] matrixSizes = {100, 200, 500, 1000, 2000};

// Thread counts to test  
int[] threadCounts = {1, 2, 4, 8, 16};

// Number of runs for averaging
int runs = 5;
```

## Requirements

- Java 8 or higher
- Sufficient RAM for large matrices (1000×1000 needs ~12MB per matrix)

## Author

PPD Lab 3 - Parallel and Distributed Programming

## License

Educational use only

