# Distributed Shared Memory (DSM) - Lab 8

A Java implementation of a simple Distributed Shared Memory mechanism demonstrating consistency concepts in distributed programming.

---

## Table of Contents

1. [What is Distributed Shared Memory?](#what-is-distributed-shared-memory)
2. [Why DSM Matters](#why-dsm-matters)
3. [Architecture Overview](#architecture-overview)
4. [Key Concepts](#key-concepts)
5. [How It Works](#how-it-works)
6. [Code Structure](#code-structure)
7. [Running the Demo](#running-the-demo)
8. [Deep Dive into the Implementation](#deep-dive-into-the-implementation)

---

## What is Distributed Shared Memory?

**Distributed Shared Memory (DSM)** is a programming abstraction that provides the illusion of a single shared memory space across multiple computers (or processes) in a distributed system.

### The Problem

In a traditional single-computer system, multiple threads can easily share data through memory:

```
┌─────────────────────────────┐
│         SHARED MEMORY       │
│    ┌──────────────────┐     │
│    │   counter = 42   │     │
│    └──────────────────┘     │
└────────▲──────────▲─────────┘
         │          │
    ┌────┴────┐ ┌───┴─────┐
    │ Thread 1│ │ Thread 2│
    └─────────┘ └─────────┘
```

But in a distributed system, each computer has its own memory:

```
  Computer A              Computer B              Computer C
┌──────────────┐       ┌──────────────┐       ┌──────────────┐
│ Memory       │       │ Memory       │       │ Memory       │
│ counter = ?  │       │ counter = ?  │       │ counter = ?  │
└──────────────┘       └──────────────┘       └──────────────┘
       │                      │                      │
       └──────────────────────┴──────────────────────┘
                         NETWORK
```

How do we keep all copies of `counter` consistent?

### The Solution: DSM

DSM provides mechanisms to:

1. **Replicate** variables across nodes
2. **Synchronize** changes when a variable is modified
3. **Guarantee ordering** so all nodes see changes in the same sequence

---

## Why DSM Matters

### Real-World Applications

1. **Distributed Databases**: Keeping data consistent across replicas
2. **Distributed Caches**: Invalidating cached values when data changes
3. **Collaborative Editing**: Multiple users editing the same document
4. **Distributed Gaming**: Keeping game state synchronized
5. **IoT Systems**: Coordinating sensor readings across devices

### The Consistency Challenge

Without proper synchronization, you can have **inconsistencies**:

```
Time    Node A              Node B
─────────────────────────────────────
T1      counter = 0         counter = 0
T2      Write counter=1     Write counter=2
T3      counter = 1         counter = 2
        (Both nodes have different values!)
```

DSM solves this by ensuring all nodes eventually see the same value and **in the same order**.

---

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                        DSM SYSTEM                                   │
│                                                                     │
│  ┌───────────────────────────────────────────────────────────────┐  │
│  │                    Node Network                                │  │
│  │                                                                │  │
│  │   ┌─────────────┐    ┌─────────────┐    ┌─────────────┐       │  │
│  │   │   Node 0    │◄──►│   Node 1    │◄──►│   Node 2    │       │  │
│  │   │             │    │             │    │             │       │  │
│  │   │ ┌─────────┐ │    │ ┌─────────┐ │    │ ┌─────────┐ │       │  │
│  │   │ │ DSM Lib │ │    │ │ DSM Lib │ │    │ │ DSM Lib │ │       │  │
│  │   │ └─────────┘ │    │ └─────────┘ │    │ └─────────┘ │       │  │
│  │   │             │    │             │    │             │       │  │
│  │   │ Variables:  │    │ Variables:  │    │ Variables:  │       │  │
│  │   │ - counter   │    │ - counter   │    │ - counter   │       │  │
│  │   │   (owner)   │    │ - flag      │    │ - flag      │       │  │
│  │   │             │    │   (owner)   │    │             │       │  │
│  │   └─────────────┘    └─────────────┘    └─────────────┘       │  │
│  │          │                  │                  │              │  │
│  │          └──────────────────┴──────────────────┘              │  │
│  │                    Message Passing                            │  │
│  └───────────────────────────────────────────────────────────────┘  │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

## Key Concepts

### 1. Variable Ownership

Each shared variable has an **owner node** that acts as the **sequencer**:

```
Variable "counter" (Owner: Node 0)
├── Node 0: OWNER (controls sequencing)
├── Node 1: Subscriber
└── Node 2: Subscriber
```

The owner:

- Receives all write requests for that variable
- Assigns a **sequence number** to each update
- Broadcasts updates to all subscribers

### 2. Subscriptions

Only nodes that **subscribe** to a variable can:

- Read its value
- Write to it (request is forwarded to owner)
- Receive change notifications

This is an optimization: messages only flow between subscribers, not all nodes.

### 3. Total Ordering

**Problem**: If Node A writes `counter=1` and Node B writes `flag=2`, different nodes might see these in different orders:

```
Node X sees: counter=1, then flag=2
Node Y sees: flag=2, then counter=1
```

**Solution**: The owner assigns **sequence numbers**:

```
Update #1: counter=1
Update #2: flag=2

All nodes apply updates in sequence order: #1, then #2
```

### 4. Compare-And-Exchange (CAS)

An **atomic operation** that:

1. Compares current value with expected value
2. If equal, sets to new value
3. Returns success/failure

Used for **lock-free synchronization**:

```java
// Atomic increment using CAS
int current = read("counter");
while (!compareAndExchange("counter", current, current + 1)) {
    current = read("counter");  // Retry with updated value
}
```

---

## How It Works

### Write Operation Flow

```
┌─────────────────────────────────────────────────────────────────────┐
│ Node 2 wants to write counter = 100                                 │
└─────────────────────────────────────────────────────────────────────┘

Step 1: Node 2 sends WRITE_REQUEST to owner (Node 0)

    Node 2                          Node 0 (Owner)
      │                                  │
      │ ──── WRITE_REQUEST ────────────► │
      │      (counter = 100)             │
      │                                  │

Step 2: Owner assigns sequence number and broadcasts

    Node 0 (Owner)
      │
      │ seq_num = 1
      │ Apply locally: counter = 100
      │
      │ ──── UPDATE_NOTIFICATION ──────► Node 1
      │      (counter=100, seq=1)
      │
      │ ──── UPDATE_NOTIFICATION ──────► Node 2
      │      (counter=100, seq=1)

Step 3: Subscribers apply update and acknowledge

    Node 1                          Node 2
      │                               │
      │ Apply: counter = 100          │ Apply: counter = 100
      │                               │
      │ ──── UPDATE_ACK ──────────►   │ ──── UPDATE_ACK ──────►
      │      (to Node 0)              │      (to Node 0)
```

### Handling Out-of-Order Messages

Network delays can cause messages to arrive out of order. The DSM buffers pending updates:

```
Node 1 receives:
  - seq=3 (arrives first due to network)
  - seq=2 (arrives second)
  - seq=1 (arrives third)

Buffer: [seq=3, seq=2, seq=1]

Apply when consecutive:
  1. Apply seq=1 ✓
  2. Apply seq=2 ✓
  3. Apply seq=3 ✓
```

---

## Code Structure

```
src/
├── dsm/                        # DSM Library Package
│   ├── MessageType.java        # Enum of all message types
│   ├── Message.java            # Message object with factory methods
│   ├── DSMCallback.java        # Callback interface for notifications
│   ├── DSMVariable.java        # Represents a shared variable
│   ├── DSMLibrary.java         # Core DSM implementation
│   └── DSMNode.java            # Node wrapper with networking
│
└── main/
    └── MainProgram.java        # Demonstration program
```

### Class Responsibilities

| Class         | Purpose                                                              |
| ------------- | -------------------------------------------------------------------- |
| `MessageType` | Defines all message types (WRITE_REQUEST, UPDATE_NOTIFICATION, etc.) |
| `Message`     | Immutable message with factory methods for each type                 |
| `DSMCallback` | Functional interface for variable change notifications               |
| `DSMVariable` | Holds value, owner ID, subscribers, sequence numbers                 |
| `DSMLibrary`  | Core logic: write handling, CAS, total ordering, callbacks           |
| `DSMNode`     | Encapsulates DSMLibrary + message queuing + threading                |

---

## Running the Demo

### Compile

```bash
cd /Users/alex/GitHub/Babes\ Bolyai\ -\ Personal/Semester5/PPD/lab8
javac -d out src/dsm/*.java src/main/*.java
```

### Run

```bash
java -cp out main.MainProgram
```

### Expected Output

The demo shows:

1. **Basic Operations**: Local write and reading from all nodes
2. **Remote Writes**: Writing through a non-owner node
3. **Total Ordering**: Verifying all nodes see updates in same order
4. **CAS Operations**: Atomic compare-and-exchange with concurrent increments

---

## Deep Dive into the Implementation

### 1. The DSMLibrary Class

This is the heart of the system. Key methods:

```java
// Register a variable you own
public void registerOwnedVariable(String name, int initialValue)

// Subscribe to a variable owned by another node
public void subscribeToVariable(String name, int ownerId, int initialValue, Set<Integer> otherSubscribers)

// Write a value (forwards to owner if needed)
public void write(String variableName, int value)

// Atomic compare-and-exchange
public boolean compareAndExchange(String varName, int expected, int newValue, long timeoutMs)

// Handle incoming messages
public void handleMessage(Message message)
```

### 2. Total Ordering Mechanism

The key to consistency is the **sequence number** assigned by the owner:

```java
private void processWriteAsOwner(String varName, int newValue, int requesterId) {
    lock.lock();
    try {
        DSMVariable var = variables.get(varName);

        // Get next sequence number (atomic increment)
        long seqNum = var.getNextSequenceNumber();

        // Apply locally
        var.setValue(newValue);
        var.setLastAppliedSeqNum(seqNum);

        // Broadcast to all subscribers with sequence number
        Message notification = Message.updateNotification(varName, newValue, seqNum, nodeId);
        for (int subscriberId : var.getSubscribers()) {
            if (subscriberId != nodeId) {
                sendMessage(notification, subscriberId);
            }
        }
    } finally {
        lock.unlock();
    }
}
```

### 3. Buffering Out-of-Order Updates

Subscribers buffer updates that arrive out of order:

```java
private void handleUpdateNotification(Message message) {
    lock.lock();
    try {
        DSMVariable var = variables.get(varName);
        PriorityQueue<PendingUpdate> pending = pendingUpdates.get(varName);

        // Add to priority queue (sorted by sequence number)
        pending.add(new PendingUpdate(seqNum, newValue));

        // Apply all consecutive updates
        while (!pending.isEmpty() &&
               pending.peek().seqNum == var.getLastAppliedSeqNum() + 1) {
            PendingUpdate update = pending.poll();
            var.setValue(update.value);
            var.setLastAppliedSeqNum(update.seqNum);
            invokeCallbacks(varName, oldValue, update.value);
        }
    } finally {
        lock.unlock();
    }
}
```

### 4. Compare-And-Exchange Atomicity

CAS is processed atomically by the owner:

```java
private boolean processCasAsOwner(String varName, int expectedValue, int newValue, int requesterId) {
    lock.lock();
    try {
        DSMVariable var = variables.get(varName);

        // Atomic check
        if (var.getValue() != expectedValue) {
            return false;  // CAS failed
        }

        // Success - update like a normal write
        long seqNum = var.getNextSequenceNumber();
        var.setValue(newValue);
        var.setLastAppliedSeqNum(seqNum);

        // Broadcast to subscribers
        broadcastUpdate(varName, newValue, seqNum);

        return true;
    } finally {
        lock.unlock();
    }
}
```

---

## Key Takeaways

1. **Ownership Pattern**: Each variable has one owner that serializes all writes
2. **Sequence Numbers**: Enable total ordering across all subscribers
3. **Buffering**: Handles network delays by buffering out-of-order updates
4. **No Central Server**: Messages only flow between subscribers
5. **CAS for Atomicity**: Compare-and-exchange enables lock-free coordination

---

## Further Reading

- **Lamport Clocks**: Logical clocks for ordering events in distributed systems
- **Vector Clocks**: Extension of Lamport clocks for causality tracking
- **Paxos/Raft**: Consensus algorithms for distributed agreement
- **CAP Theorem**: Trade-offs between Consistency, Availability, Partition tolerance

---

## License

This is an educational implementation for Babeș-Bolyai University's Parallel and Distributed Programming course (PPD).
