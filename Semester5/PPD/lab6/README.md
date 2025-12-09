# Lab 6: Parallel Hamiltonian Cycle Finder

## Problem Description

Given a directed graph, find a Hamiltonian cycle (a cycle that visits every vertex exactly once and returns to the starting vertex). The implementation uses parallel algorithms to speed up the search.

## Project Structure

```
src/main/java/hamiltonian/
├── Graph.java                     # Directed graph representation
├── HamiltonianCycleFinder.java    # Interface for cycle finders
├── HamiltonianResult.java         # Result container with timing
├── SequentialHamiltonianFinder.java   # Baseline sequential implementation
├── ParallelHamiltonianFinder.java     # Manual thread management implementation
├── ForkJoinHamiltonianFinder.java     # ForkJoinPool with RecursiveTask
└── Main.java                      # Benchmark runner
```

## Algorithms

### 1. Sequential Backtracking (Baseline)

The sequential algorithm uses classic backtracking to explore the search space:

```
function backtrack(path, visited):
    if path.size == n:
        if edge exists from last vertex to start:
            return path + start  // Found Hamiltonian cycle
        return null

    for each neighbor of current vertex:
        if not visited[neighbor]:
            visited[neighbor] = true
            path.add(neighbor)
            result = backtrack(path, visited)
            if result != null:
                return result
            path.removeLast()
            visited[neighbor] = false

    return null
```

**Time Complexity:** O(n!) in the worst case
**Space Complexity:** O(n) for the recursion stack and visited array

### 2. Parallel Implementation (Manual Threads)

This implementation distributes threads across branches at each level of the search tree.

**Thread Distribution Strategy:**

- If we have `T` threads and `B` unvisited neighbors at a vertex:
  - Each branch gets approximately `T/B` threads
  - Extra threads go to the first branches
  - Example: 8 threads, 3 branches → [3, 3, 2] distribution

**Algorithm:**

```
function parallelSearch(path, visited, availableThreads):
    if solution found:
        return

    if path.size == n:
        check and record solution if valid
        return

    unvisitedNeighbors = filter unvisited neighbors

    if availableThreads <= 1 or only one neighbor:
        sequentialSearch(path, visited)  // Fall back to sequential
        return

    threadsPerBranch = distributeThreads(availableThreads, unvisitedNeighbors.size)

    for each branch:
        create new thread with copied state
        allocate threadsPerBranch[i] threads to this branch

    start all threads and wait for completion
```

### 3. ForkJoinPool Implementation (RecursiveTask)

This implementation uses Java's Fork/Join framework for automatic work stealing and load balancing.

**Strategy:**

- Each branch exploration is a `RecursiveTask<List<Integer>>`
- Tasks fork sub-tasks for each unvisited neighbor
- Uses a sequential threshold: when remaining vertices ≤ 3, switch to sequential
- The last branch is computed directly (avoiding unnecessary fork overhead)

**Algorithm:**

```
class HamiltonianTask extends RecursiveTask<List<Integer>>:
    function compute():
        if solution already found:
            return null

        if path.size == n:
            if valid cycle:
                mark found and return cycle
            return null

        if close to end or single branch:
            return sequentialSearch()

        // Fork all branches except last
        for i = 0 to neighbors.size - 2:
            fork new HamiltonianTask for neighbor[i]

        // Compute last branch directly
        result = compute task for last neighbor

        // Join forked tasks
        for each forked task:
            join and collect result

        return result
```

## Synchronization Mechanisms

### Manual Thread Implementation

1. **AtomicReference<List<Integer>>** for the found cycle:

   - Thread-safe sharing of the result across all threads
   - Uses `compareAndSet(null, cycle)` to ensure only one thread sets the result
   - All threads check this reference before continuing work

2. **State Isolation:**

   - Each thread works with its own copy of `visited[]` and `path`
   - No race conditions on state since each branch has independent copies
   - Memory overhead is O(threads × n) for the visited arrays

3. **Thread Coordination:**
   - Parent thread creates child threads and calls `join()` on all
   - Early termination: threads check `foundCycle.get() != null` periodically

### ForkJoinPool Implementation

1. **AtomicBoolean** for solution found flag:

   - Lighter weight than AtomicReference for just signaling
   - All tasks check this before and during computation

2. **ForkJoinPool Work Stealing:**

   - Idle workers steal tasks from busy workers' queues
   - Automatic load balancing without explicit thread management
   - More efficient for irregular workloads (common in backtracking)

3. **Task Cancellation:**
   - When a solution is found, remaining tasks are cancelled
   - `task.cancel(true)` called on forked tasks when result is found

## Performance Measurements

### How to Run

```bash
cd src/main/java
javac hamiltonian/*.java
java hamiltonian.Main
```

### Expected Output Format

```
================================================================================
  HAMILTONIAN CYCLE FINDER - PARALLEL ALGORITHMS BENCHMARK
================================================================================

Graph Size: 14 vertices
--------------------------------------------------------------------------------
Algorithm                           |     Avg Time (ms) |    Speedup | Result
-------------------------------------------------------------------------------------
Sequential Backtracking             |           150.234 |      1.00x | ✓ Valid cycle
Parallel (2 threads)                |            82.156 |      1.83x | ✓ Valid cycle
ForkJoin (2 parallelism)            |            78.432 |      1.92x | ✓ Valid cycle
Parallel (4 threads)                |            45.678 |      3.29x | ✓ Valid cycle
ForkJoin (4 parallelism)            |            42.123 |      3.57x | ✓ Valid cycle
Parallel (8 threads)                |            28.456 |      5.28x | ✓ Valid cycle
ForkJoin (8 parallelism)            |            25.789 |      5.83x | ✓ Valid cycle
```

### Performance Analysis

**Factors Affecting Speedup:**

1. **Graph Structure:**

   - Dense graphs have more neighbors → better parallelization opportunities
   - Sparse graphs may have limited branching → less parallelism benefit

2. **Graph Size:**

   - Small graphs (< 10 vertices): Overhead may exceed benefits
   - Medium graphs (10-16 vertices): Good speedup observable
   - Large graphs (16+ vertices): Significant speedup, but base time is long

3. **Thread Count vs. Branching Factor:**

   - Best speedup when thread count ≈ average branching factor
   - Too many threads → overhead from thread creation and coordination
   - Too few threads → underutilization of available parallelism

4. **Solution Location:**
   - If Hamiltonian cycle is found early, less total work is done
   - Parallel search may find the solution faster by exploring multiple paths

**Why ForkJoinPool Often Performs Better:**

1. **Work Stealing:** Automatically balances load when some branches finish early
2. **Less Overhead:** Reuses threads from the pool instead of creating new ones
3. **Better Cache Locality:** Related tasks are often processed by the same thread

### Typical Speedup Observations

| Threads | Expected Speedup (14 vertices) |
| ------- | ------------------------------ |
| 2       | 1.5x - 2.0x                    |
| 4       | 2.5x - 4.0x                    |
| 8       | 4.0x - 6.0x                    |

_Note: Actual speedup depends heavily on graph structure and hardware._

## Implementation Notes

### Graph Generation

- `Graph.createWithHamiltonianCycle(n, extraEdges, random)`: Creates a graph guaranteed to have at least one Hamiltonian cycle
- First creates a random permutation cycle, then adds extra random edges
- Useful for testing to ensure a solution exists

### Verification

The `Main` class includes a `verifyCycle()` method that validates:

1. Cycle starts and ends at the start vertex
2. All vertices are visited exactly once
3. All edges in the cycle exist in the graph

## Future Improvements

1. **Pruning Optimizations:**

   - Check for dead ends earlier
   - Use heuristics to order neighbor exploration

2. **Better Thread Distribution:**

   - Consider estimated subtree sizes
   - Dynamic rebalancing during execution

3. **Memory Optimizations:**
   - Use bitsets instead of boolean arrays
   - Share read-only graph data more efficiently
