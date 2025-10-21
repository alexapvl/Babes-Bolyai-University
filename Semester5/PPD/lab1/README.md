# Warehouse Synchronization Problem

This project is a Java implementation for the "Warehouses" problem, which demonstrates the use of threads and mutexes for concurrent operations on shared data.

## Problem Description

A wholesaler has several warehouses storing various products. The system must track the quantity of each product in each warehouse. Multiple concurrent "move" operations transfer products between warehouses. The main goal is to ensure data consistency using proper synchronization, such that the total quantity of each product across all warehouses remains constant.

## Design and Synchronization

The solution is designed with a fine-grained locking strategy to allow for high concurrency.

### Domain Model

- `Warehouse`: Represents a single warehouse. It stores product quantities in a `Map<Integer, Integer>`.
- `Inventory`: Manages all warehouses. It is responsible for orchestrating move operations and performing system-wide inventory checks.
- `MoveWorker`: A `Runnable` task that simulates a user performing a series of random product moves.
- `Main`: The main class that sets up the simulation, starts the worker threads, and reports the results.

### Synchronization Strategy

The key challenge is to perform a `move` operation (transferring a product from a source to a destination warehouse) without causing race conditions or deadlocks.

- **Mutex per Warehouse**: Each `Warehouse` object has its own `java.util.concurrent.locks.ReentrantLock`. This is a fine-grained approach. A move operation only needs to lock the two warehouses involved, allowing other threads to work on different warehouses concurrently.

- **Deadlock Avoidance**: A `move` operation requires acquiring locks on both the source and destination warehouses. If two threads try to move products between the same two warehouses but in opposite directions (e.g., Thread 1 moves from A to B, Thread 2 moves from B to A), they could deadlock. Thread 1 locks A and waits for B, while Thread 2 locks B and waits for A.

  To prevent this, a strict lock acquisition order is enforced. Before locking, the warehouses are ordered by their unique IDs. The lock for the warehouse with the lower ID is always acquired first. This strategy breaks the circular wait condition required for a deadlock.

- **Inventory Check**: The inventory check must provide a consistent view of the entire system. To achieve this, it locks all warehouses before summing up the product quantities. This temporarily blocks all move operations, ensuring that the check is performed on a stable snapshot of the data. The locks are also acquired in a fixed order (by warehouse ID) to remain consistent.

## Performance

The performance of the system is evaluated by measuring the time it takes to complete a large number of move operations with a varying number of threads.

### Test Environment

- **Hardware**: M1 Pro, 16GB RAM
- **OS**: macOS 26.0.1
- **JVM**: OpenJDK 23

### Results

| Number of Threads | Total Operations | Time (ms) |
| ----------------- | ---------------- | --------- |
| 1                 | 100,000          | 25        |
| 2                 | 200,000          | 58        |
| 4                 | 400,000          | 86        |
| 8                 | 800,000          | 152       |
| 16                | 1,600,000        | 266       |

The results are expected to show that increasing the number of threads improves performance up to a certain point, corresponding to the number of available CPU cores. Beyond that, the overhead from context switching and lock contention may lead to diminishing returns or even performance degradation.

## How to Run

1. Compile and rum the Java files:
   ```sh
   javac src/main/java/org/example/*.java && java -cp src/main/java/org/example Main
   ```
