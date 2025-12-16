package main;

import dsm.*;

import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Main demonstration program for the Distributed Shared Memory system.
 * 
 * This program creates multiple DSM nodes and demonstrates:
 * 1. Variable registration and subscription
 * 2. Write operations (local and remote)
 * 3. Callback notifications with total ordering
 * 4. Compare-and-exchange (CAS) operations
 * 
 * The demonstration uses 3 nodes with 2 shared variables:
 * - "counter": Owned by Node 0, subscribed by all nodes
 * - "flag": Owned by Node 1, subscribed by Nodes 1 and 2
 */
public class MainProgram {

  // Configuration
  private static final int NUM_NODES = 3;
  private static final int NETWORK_DELAY_MS = 10; // Simulated network delay

  // Nodes
  private static DSMNode[] nodes;
  private static Map<Integer, DSMNode> nodeNetwork;

  // For tracking callback order
  private static List<String> node0Callbacks = Collections.synchronizedList(new ArrayList<>());
  private static List<String> node1Callbacks = Collections.synchronizedList(new ArrayList<>());
  private static List<String> node2Callbacks = Collections.synchronizedList(new ArrayList<>());

  public static void main(String[] args) throws Exception {
    System.out.println("╔══════════════════════════════════════════════════════════════════╗");
    System.out.println("║     DISTRIBUTED SHARED MEMORY (DSM) DEMONSTRATION                ║");
    System.out.println("╚══════════════════════════════════════════════════════════════════╝");
    System.out.println();

    // Initialize nodes
    initializeNodes();

    // Set up variables and subscriptions
    setupVariables();

    // Start all nodes
    startNodes();

    // Wait for nodes to be ready
    Thread.sleep(100);

    // Run demonstrations
    demonstrateBasicOperations();
    demonstrateRemoteWrites();
    demonstrateTotalOrdering();
    demonstrateCAS();

    // Cleanup
    stopNodes();

    System.out.println();
    System.out.println("╔══════════════════════════════════════════════════════════════════╗");
    System.out.println("║     DEMONSTRATION COMPLETED SUCCESSFULLY                         ║");
    System.out.println("╚══════════════════════════════════════════════════════════════════╝");
  }

  /**
   * Creates and configures all DSM nodes.
   */
  private static void initializeNodes() {
    System.out.println("▶ Initializing " + NUM_NODES + " DSM nodes...");

    nodes = new DSMNode[NUM_NODES];
    nodeNetwork = new ConcurrentHashMap<>();

    for (int i = 0; i < NUM_NODES; i++) {
      nodes[i] = new DSMNode(i, NUM_NODES, NETWORK_DELAY_MS);
      nodeNetwork.put(i, nodes[i]);
    }

    // Connect all nodes to the network
    for (DSMNode node : nodes) {
      node.setNodeNetwork(nodeNetwork);
    }

    System.out.println("  ✓ Created " + NUM_NODES + " nodes with " + NETWORK_DELAY_MS + "ms simulated network delay");
    System.out.println();
  }

  /**
   * Sets up shared variables and their subscriptions.
   * 
   * Variable configuration:
   * - "counter": Owner = Node 0, Subscribers = Nodes 0, 1, 2
   * - "flag": Owner = Node 1, Subscribers = Nodes 1, 2
   */
  private static void setupVariables() {
    System.out.println("▶ Setting up shared variables...");

    // Variable "counter" - owned by Node 0, subscribed by all
    // Owner must know about all subscribers to broadcast updates
    nodes[0].registerVariable("counter", 0, Set.of(1, 2));
    nodes[1].subscribe("counter", 0, 0, Set.of(0, 2));
    nodes[2].subscribe("counter", 0, 0, Set.of(0, 1));

    // Variable "flag" - owned by Node 1, subscribed by Nodes 1 and 2
    nodes[1].registerVariable("flag", 0, Set.of(2));
    nodes[2].subscribe("flag", 1, 0, Set.of(1));

    // Register callbacks to track changes
    nodes[0].onVariableChange("counter", (name, oldVal, newVal) -> {
      String entry = name + ": " + oldVal + " -> " + newVal;
      node0Callbacks.add(entry);
      System.out.println("  [Callback Node 0] " + entry);
    });

    nodes[1].onVariableChange("counter", (name, oldVal, newVal) -> {
      String entry = name + ": " + oldVal + " -> " + newVal;
      node1Callbacks.add(entry);
      System.out.println("  [Callback Node 1] " + entry);
    });
    nodes[1].onVariableChange("flag", (name, oldVal, newVal) -> {
      String entry = name + ": " + oldVal + " -> " + newVal;
      node1Callbacks.add(entry);
      System.out.println("  [Callback Node 1] " + entry);
    });

    nodes[2].onVariableChange("counter", (name, oldVal, newVal) -> {
      String entry = name + ": " + oldVal + " -> " + newVal;
      node2Callbacks.add(entry);
      System.out.println("  [Callback Node 2] " + entry);
    });
    nodes[2].onVariableChange("flag", (name, oldVal, newVal) -> {
      String entry = name + ": " + oldVal + " -> " + newVal;
      node2Callbacks.add(entry);
      System.out.println("  [Callback Node 2] " + entry);
    });

    System.out.println("  ✓ Variable 'counter' (owner: Node 0, subscribers: all nodes)");
    System.out.println("  ✓ Variable 'flag' (owner: Node 1, subscribers: Nodes 1, 2)");
    System.out.println();
  }

  private static void startNodes() {
    System.out.println("▶ Starting all nodes...");
    for (DSMNode node : nodes) {
      node.start();
    }
    System.out.println();
  }

  private static void stopNodes() {
    System.out.println("▶ Stopping all nodes...");
    for (DSMNode node : nodes) {
      node.stop();
    }
  }

  // ============== Demonstrations ==============

  /**
   * Demonstrates basic read and write operations.
   */
  private static void demonstrateBasicOperations() throws Exception {
    System.out.println("═══════════════════════════════════════════════════════════════════");
    System.out.println("  DEMO 1: Basic Read/Write Operations");
    System.out.println("═══════════════════════════════════════════════════════════════════");
    System.out.println();

    // Local write (Node 0 owns counter)
    System.out.println("→ Node 0 writes 'counter' = 42 (local write, Node 0 is owner)");
    nodes[0].write("counter", 42);

    // Wait for propagation
    Thread.sleep(200);

    // Read from all nodes
    System.out.println();
    System.out.println("→ Reading 'counter' from all nodes:");
    System.out.println("  Node 0: counter = " + nodes[0].read("counter"));
    System.out.println("  Node 1: counter = " + nodes[1].read("counter"));
    System.out.println("  Node 2: counter = " + nodes[2].read("counter"));
    System.out.println();
  }

  /**
   * Demonstrates remote write operations.
   */
  private static void demonstrateRemoteWrites() throws Exception {
    System.out.println("═══════════════════════════════════════════════════════════════════");
    System.out.println("  DEMO 2: Remote Write Operations");
    System.out.println("═══════════════════════════════════════════════════════════════════");
    System.out.println();

    // Remote write (Node 2 writes to counter owned by Node 0)
    System.out.println("→ Node 2 writes 'counter' = 100 (remote write, forwarded to Node 0)");
    nodes[2].write("counter", 100);

    // Wait for propagation
    Thread.sleep(200);

    System.out.println();
    System.out.println("→ Reading 'counter' from all nodes:");
    System.out.println("  Node 0: counter = " + nodes[0].read("counter"));
    System.out.println("  Node 1: counter = " + nodes[1].read("counter"));
    System.out.println("  Node 2: counter = " + nodes[2].read("counter"));
    System.out.println();
  }

  /**
   * Demonstrates total ordering of callbacks.
   * All nodes should see changes in the same order.
   */
  private static void demonstrateTotalOrdering() throws Exception {
    System.out.println("═══════════════════════════════════════════════════════════════════");
    System.out.println("  DEMO 3: Total Ordering of Updates");
    System.out.println("═══════════════════════════════════════════════════════════════════");
    System.out.println();
    System.out.println("  This demonstrates that all nodes see updates in the same order.");
    System.out.println("  We'll perform multiple rapid writes and verify callback ordering.");
    System.out.println();

    // Clear previous callbacks
    node0Callbacks.clear();
    node1Callbacks.clear();
    node2Callbacks.clear();

    // Perform multiple writes rapidly
    System.out.println("→ Performing 5 rapid writes to 'counter'...");
    for (int i = 1; i <= 5; i++) {
      nodes[0].write("counter", i * 10);
      Thread.sleep(50); // Small delay to ensure ordering
    }

    // Wait for all updates to propagate
    Thread.sleep(500);

    System.out.println();
    System.out.println("→ Callback order verification:");
    System.out.println("  Node 0 saw: " + node0Callbacks);
    System.out.println("  Node 1 saw: " + node1Callbacks);
    System.out.println("  Node 2 saw: " + node2Callbacks);

    // Verify ordering is the same
    boolean orderingCorrect = verifyCallbackOrdering("counter");
    System.out.println();
    System.out.println("  Total ordering " + (orderingCorrect ? "✓ VERIFIED" : "✗ FAILED"));
    System.out.println();
  }

  /**
   * Demonstrates Compare-And-Exchange (CAS) operations.
   */
  private static void demonstrateCAS() throws Exception {
    System.out.println("═══════════════════════════════════════════════════════════════════");
    System.out.println("  DEMO 4: Compare-And-Exchange (CAS) Operations");
    System.out.println("═══════════════════════════════════════════════════════════════════");
    System.out.println();

    // Reset counter to known value
    nodes[0].write("counter", 0);
    Thread.sleep(300); // Wait for propagation to all nodes

    System.out.println("→ Counter reset to 0 (all nodes synchronized)");
    System.out.println("  Node 0: " + nodes[0].read("counter"));
    System.out.println("  Node 1: " + nodes[1].read("counter"));
    System.out.println("  Node 2: " + nodes[2].read("counter"));
    System.out.println();

    // Successful CAS
    System.out.println("→ Node 1 attempts CAS: if counter==0, set to 99");
    boolean success = nodes[1].compareAndExchange("counter", 0, 99, 1000);
    Thread.sleep(300);
    System.out.println("  Result: " + (success ? "SUCCESS" : "FAILED"));
    System.out.println("  Counter value on all nodes: " + nodes[0].read("counter"));
    System.out.println();

    // Failed CAS (wrong expected value)
    System.out.println("→ Node 2 attempts CAS: if counter==0, set to 50 (should fail, counter is 99)");
    success = nodes[2].compareAndExchange("counter", 0, 50, 1000);
    Thread.sleep(300);
    System.out.println("  Result: " + (success ? "SUCCESS" : "FAILED (expected)"));
    System.out.println("  Counter value: " + nodes[0].read("counter"));
    System.out.println();

    // Demonstrate atomic increment using CAS - owner only
    System.out.println("→ Demonstrating atomic increment with CAS (owner node only):");
    nodes[0].write("counter", 0);
    Thread.sleep(300);

    int numIncrements = 5;
    System.out.println("  Performing " + numIncrements + " atomic increments using CAS loop");

    for (int i = 0; i < numIncrements; i++) {
      boolean done = false;
      int attempts = 0;
      while (!done && attempts < 10) {
        int current = nodes[0].read("counter");
        done = nodes[0].compareAndExchange("counter", current, current + 1, 1000);
        attempts++;
        if (done) {
          System.out.println("    Increment " + (i + 1) + ": " + current + " → " + (current + 1));
        }
      }
    }

    Thread.sleep(300);

    int actualValue = nodes[0].read("counter");
    System.out.println();
    System.out.println("  Expected final value: " + numIncrements);
    System.out.println("  Actual final value:   " + actualValue);
    System.out.println("  CAS atomicity " + (actualValue == numIncrements ? "✓ VERIFIED" : "✗ FAILED"));
    System.out.println();

    // Demonstrate CAS conflict between two nodes
    System.out.println("→ Demonstrating CAS conflict resolution:");
    nodes[0].write("counter", 0);
    Thread.sleep(300);

    System.out.println("  Both Node 1 and Node 2 try to set counter from 0 to their ID");
    System.out.println("  Only one should succeed (atomic operation)");

    CountDownLatch startLatch = new CountDownLatch(1);
    CountDownLatch doneLatch = new CountDownLatch(2);
    AtomicInteger node1Result = new AtomicInteger(-1);
    AtomicInteger node2Result = new AtomicInteger(-1);

    new Thread(() -> {
      try {
        startLatch.await();
        boolean result = nodes[1].compareAndExchange("counter", 0, 1, 1000);
        node1Result.set(result ? 1 : 0);
      } catch (Exception e) {
        e.printStackTrace();
      }
      doneLatch.countDown();
    }).start();

    new Thread(() -> {
      try {
        startLatch.await();
        boolean result = nodes[2].compareAndExchange("counter", 0, 2, 1000);
        node2Result.set(result ? 1 : 0);
      } catch (Exception e) {
        e.printStackTrace();
      }
      doneLatch.countDown();
    }).start();

    startLatch.countDown(); // Start both at the same time
    doneLatch.await(5, TimeUnit.SECONDS);
    Thread.sleep(300);

    System.out.println("  Node 1 CAS result: " + (node1Result.get() == 1 ? "SUCCESS" : "FAILED"));
    System.out.println("  Node 2 CAS result: " + (node2Result.get() == 1 ? "SUCCESS" : "FAILED"));
    System.out.println("  Final counter value: " + nodes[0].read("counter"));

    boolean exactlyOneSucceeded = (node1Result.get() + node2Result.get()) == 1;
    System.out
        .println("  CAS atomicity " + (exactlyOneSucceeded ? "✓ VERIFIED (exactly one succeeded)" : "✗ CHECK RESULTS"));
    System.out.println();
  }

  /**
   * Verifies that all nodes saw the same ordering for a variable's updates.
   */
  private static boolean verifyCallbackOrdering(String varName) {
    // Filter callbacks for the specific variable
    List<String> filtered0 = node0Callbacks.stream()
        .filter(s -> s.startsWith(varName)).toList();
    List<String> filtered1 = node1Callbacks.stream()
        .filter(s -> s.startsWith(varName)).toList();
    List<String> filtered2 = node2Callbacks.stream()
        .filter(s -> s.startsWith(varName)).toList();

    // All nodes that subscribe to counter should see the same order
    return filtered0.equals(filtered1) && filtered1.equals(filtered2);
  }
}
