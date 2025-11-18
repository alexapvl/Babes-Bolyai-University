# Lab 4: HTTP Downloader - Three Implementations

## Overview

This project demonstrates three different approaches to implementing asynchronous HTTP file downloads in C# using low-level socket operations. Each implementation uses the same underlying socket API (`BeginConnect/EndConnect`, `BeginSend/EndSend`, `BeginReceive/EndReceive`) but differs in how it handles asynchronous operations and control flow.

## Project Structure

```
lab4/
├── HttpDownloader.csproj          # Project configuration
├── Program.cs                     # Main entry point, tests all implementations
├── HttpParser.cs                  # HTTP protocol parser utilities
├── CallbackDownloader.cs          # Implementation 1: Event-driven callbacks
├── TaskDownloader.cs              # Implementation 2: Task-based with ContinueWith()
├── AsyncAwaitDownloader.cs        # Implementation 3: Async/Await
└── README.md                      # This file
```

## The Three Implementations

### Implementation 1: Callback-based (Event-Driven)

**File:** `CallbackDownloader.cs`

**Approach:**

- Directly implements the HTTP download logic in callback functions
- Each asynchronous operation (Connect, Send, Receive) has a corresponding callback
- State transitions are handled explicitly in callbacks
- Uses a state machine pattern where each callback decides what to do next

**Key Characteristics:**

- **Control Flow:** Non-linear, spread across multiple callback methods
- **State Management:** Manual state tracking using a `DownloadState` object
- **Error Handling:** Try-catch in each callback
- **Complexity:** Most verbose, requires careful state management

**How It Works:**

1. `DownloadAsync()` initializes state and calls `BeginConnect()`
2. `ConnectCallback()` is invoked when connection completes → calls `BeginSend()`
3. `SendCallback()` is invoked when send completes → calls `BeginReceive()`
4. `ReceiveCallback()` is invoked when data arrives → parses data, calls `BeginReceive()` again
5. When `bytesReceived == 0`, connection is closed → completes the `TaskCompletionSource`

**Pros:**

- Maximum control over execution flow
- No additional abstractions or wrappers
- Shows the "raw" asynchronous programming model

**Cons:**

- Difficult to read and maintain
- Callback hell potential
- Error handling scattered across multiple methods
- State must be manually passed through callbacks

**Code Example:**

```csharp
socket.BeginConnect(endPoint, ConnectCallback, state);

private static void ConnectCallback(IAsyncResult ar)
{
    var state = (DownloadState)ar.AsyncState!;
    socket.EndConnect(ar);
    // ... prepare request ...
    socket.BeginSend(requestBytes, 0, requestBytes.Length,
                     SocketFlags.None, SendCallback, state);
}
```

---

### Implementation 2: Task-based with ContinueWith()

**File:** `TaskDownloader.cs`

**Approach:**

- Wraps each `Begin/End` operation in a `Task` using `TaskCompletionSource`
- Chains operations using `ContinueWith()` method
- Each continuation returns a new task, creating a chain
- Uses `Unwrap()` to flatten nested tasks

**Key Characteristics:**

- **Control Flow:** Sequential chain of continuations
- **State Management:** State object shared across continuations
- **Error Handling:** Propagated through task chain, handled at the end
- **Complexity:** Medium, requires understanding of task continuations

**How It Works:**

1. `ConnectAsync()` wraps `BeginConnect/EndConnect` in a `TaskCompletionSource`
2. Returns a `Task` that completes when connection is established
3. `.ContinueWith()` chains the next operation (Send)
4. Each continuation checks `IsFaulted` and chains the next operation
5. Final continuation processes results or errors

**Pros:**

- Better than raw callbacks, more composable
- Clear dependency chain between operations
- Error propagation through task infrastructure
- Can specify continuation options (`OnlyOnRanToCompletion`, etc.)

**Cons:**

- Still somewhat complex with nested continuations
- Requires `Unwrap()` for tasks returning tasks
- Not as readable as async/await
- Easy to create deeply nested structures

**Code Example:**

```csharp
return ConnectAsync(socket, endPoint)
    .ContinueWith(connectTask => {
        // Handle connection result
        return SendAsync(socket, requestBytes);
    }, TaskContinuationOptions.OnlyOnRanToCompletion)
    .Unwrap()
    .ContinueWith(sendTask => {
        // Handle send result
        return ReceiveAllAsync(state);
    })
    .Unwrap();
```

**Key Wrapper Pattern:**

```csharp
private static Task<int> SendAsync(Socket socket, byte[] buffer)
{
    var tcs = new TaskCompletionSource<int>();

    socket.BeginSend(buffer, 0, buffer.Length, SocketFlags.None, ar =>
    {
        try
        {
            int bytesSent = socket.EndSend(ar);
            tcs.SetResult(bytesSent);  // Complete the task
        }
        catch (Exception ex)
        {
            tcs.SetException(ex);  // Fail the task
        }
    }, null);

    return tcs.Task;
}
```

---

### Implementation 3: Async/Await

**File:** `AsyncAwaitDownloader.cs`

**Approach:**

- Same task wrappers as Implementation 2
- Uses `async/await` keywords for cleaner syntax
- Code looks sequential but executes asynchronously
- Compiler transforms async methods into state machines

**Key Characteristics:**

- **Control Flow:** Linear, sequential-looking code
- **State Management:** Handled automatically by compiler
- **Error Handling:** Standard try-catch blocks
- **Complexity:** Lowest, most readable

**How It Works:**

1. Method marked with `async` keyword
2. Each asynchronous operation is `await`ed
3. When `await` is encountered, method returns to caller
4. When awaited task completes, execution resumes after the `await`
5. Compiler generates state machine behind the scenes

**Pros:**

- **Most readable and maintainable**
- Looks like synchronous code
- Natural error handling with try-catch
- Compiler handles state machine generation
- Modern C# best practice

**Cons:**

- Hides some complexity (can be good or bad for learning)
- Requires understanding of async/await semantics
- Slightly less control than explicit task management

**Code Example:**

```csharp
public static async Task<DownloadResult> DownloadAsync(string url)
{
    try
    {
        var (host, path, port) = HttpParser.ParseUrl(url);
        var addresses = await Dns.GetHostAddressesAsync(host);

        socket = new Socket(...);
        await ConnectAsync(socket, endPoint);

        int bytesSent = await SendAsync(socket, requestBytes);

        var (responseData, totalBytes) = await ReceiveAllAsync(socket, host);

        // Process results...
        return new DownloadResult { ... };
    }
    catch (Exception ex)
    {
        return new DownloadResult { Success = false, ... };
    }
}
```

Notice how this reads almost like synchronous code!

---

## HTTP Parser Implementation

**File:** `HttpParser.cs`

The HTTP parser is intentionally simple and handles only what's needed for this lab:

- **`ParseHeaders()`**: Extracts HTTP headers from response, finds `Content-Length`
- **`GetContentLength()`**: Parses the `Content-Length` header value
- **`BuildHttpRequest()`**: Constructs a minimal HTTP/1.1 GET request
- **`ParseUrl()`**: Extracts host, path, and port from URL

**HTTP Request Format:**

```http
GET /path HTTP/1.1
Host: example.com
User-Agent: SimpleHttpDownloader/1.0
Connection: close

```

**HTTP Response Parsing:**

- Looks for `\r\n\r\n` to find end of headers
- Splits headers by `\r\n` and parses `Key: Value` pairs
- Calculates header length to determine where body starts

---

## Running the Project

### Prerequisites

- .NET 8.0 SDK or later
- C# 12 support

### Build and Run

```bash
cd lab4
dotnet build
dotnet run
```

### Expected Output

The program will:

1. Download 3 files simultaneously using the callback approach
2. Download 3 files simultaneously using the task-based approach
3. Download 3 files simultaneously using the async/await approach
4. Display timing and success statistics for each implementation

```
HTTP Downloader - Three Implementations Demo
==============================================

Test URLs:
  - http://www.example.com/
  - http://www.example.org/
  - http://www.example.net/

========================================
Implementation 1: Callback-based
========================================
Starting Callback downloads at 14:30:45.123
[Callback] Connected to www.example.com
[Callback] Sent 89 bytes to www.example.com
[Callback] Headers parsed for www.example.com, Content-Length: 1256
...
```

---

## Key Concepts Demonstrated

### 1. Asynchronous I/O

All three implementations use **non-blocking I/O** operations. When a socket operation is initiated, the thread is free to do other work while waiting for the operation to complete.

### 2. TaskCompletionSource

Implementations 2 and 3 use `TaskCompletionSource<T>` to bridge the callback-based API to the task-based API:

```csharp
var tcs = new TaskCompletionSource<int>();
socket.BeginReceive(..., ar => {
    tcs.SetResult(socket.EndReceive(ar));
}, null);
return tcs.Task;
```

### 3. Task Continuations

Implementation 2 demonstrates how tasks can be chained:

```csharp
task1.ContinueWith(t => task2)
     .Unwrap()
     .ContinueWith(t => task3)
```

### 4. Async State Machines

Implementation 3 shows how the compiler transforms async/await into a state machine, similar to what we wrote manually in Implementation 1.

### 5. Concurrent Operations

All implementations can handle multiple downloads simultaneously. The program starts all downloads at once and uses `Task.WhenAll()` to wait for completion.

---

## Comparison Table

| Feature              | Callback               | Task (ContinueWith)              | Async/Await               |
| -------------------- | ---------------------- | -------------------------------- | ------------------------- |
| **Readability**      | ⭐ Low                 | ⭐⭐ Medium                      | ⭐⭐⭐ High               |
| **Maintainability**  | ⭐ Low                 | ⭐⭐ Medium                      | ⭐⭐⭐ High               |
| **Error Handling**   | Scattered              | Propagated                       | Natural                   |
| **Control Flow**     | Non-linear             | Chained                          | Sequential                |
| **State Management** | Manual                 | Shared object                    | Automatic                 |
| **Learning Value**   | High (shows internals) | High (shows TPL)                 | Medium (hides complexity) |
| **Real-world Use**   | Legacy code            | Rare (superseded by async/await) | **Modern standard**       |
| **Lines of Code**    | ~180                   | ~200                             | ~140                      |

---

## Learning Outcomes

After studying these implementations, you should understand:

1. **Why async/await exists**: It's syntactic sugar that makes asynchronous code much easier to write and maintain

2. **How tasks work internally**: The wrappers in implementations 2 and 3 show how `TaskCompletionSource` bridges callback-based and task-based APIs

3. **Evolution of async programming in C#**:

   - Event-driven callbacks (oldest)
   - Task-based Asynchronous Pattern (TAP) with `ContinueWith`
   - Async/await (modern)

4. **When to use each approach**:
   - **Callbacks**: When working with legacy APIs or learning fundamentals
   - **ContinueWith**: When you need fine-grained control over continuation behavior
   - **Async/await**: Default choice for all modern C# asynchronous code

---

## Common Pitfalls and Notes

### No Wait() in Async Code

The only `Wait()` or `WaitAll()` call should be in `Main()` to wait for all downloads to complete. Inside async methods, always use `await` instead of `Wait()` to avoid deadlocks.

### HTTP vs HTTPS

This implementation uses plain HTTP (not HTTPS) because SSL/TLS would add significant complexity. For production code, use `HttpClient` which handles HTTPS transparently.

### Error Handling

All three implementations handle errors gracefully and return a `DownloadResult` object indicating success or failure rather than throwing exceptions to the caller.

### Buffer Size

The 8KB buffer size (`byte[8192]`) is a reasonable default. Larger buffers can improve throughput for large files but increase memory usage.

### Connection: close

The HTTP request includes `Connection: close` header, telling the server to close the connection after sending the response. This simplifies the implementation since we know when to stop receiving (when we get 0 bytes).

---

## References

- [Task-based Asynchronous Pattern (TAP)](https://docs.microsoft.com/en-us/dotnet/standard/asynchronous-programming-patterns/task-based-asynchronous-pattern-tap)
- [Async/Await Best Practices](https://docs.microsoft.com/en-us/archive/msdn-magazine/2013/march/async-await-best-practices-in-asynchronous-programming)
- [Socket Class Documentation](https://docs.microsoft.com/en-us/dotnet/api/system.net.sockets.socket)
- [HTTP/1.1 Specification (RFC 2616)](https://www.rfc-editor.org/rfc/rfc2616)

---

## Implementation Techniques Comparison

The following table compares the most significant implementation techniques used in each solution:

| Implementation Aspect         | Callback (Event-Driven)                                                      | Task-based (ContinueWith)                                                       | Async/Await                                                       |
| ----------------------------- | ---------------------------------------------------------------------------- | ------------------------------------------------------------------------------- | ----------------------------------------------------------------- |
| **Async Operation Wrapping**  | None - uses raw `Begin*/End*` methods directly                               | Wraps `Begin*/End*` in `Task<T>` using `TaskCompletionSource<T>`                | Same wrappers as Task-based approach                              |
| **Operation Chaining**        | Manual chaining through callbacks - each callback invokes the next operation | `.ContinueWith()` method chains tasks together with `Unwrap()` for nested tasks | `await` keyword - compiler generates state machine                |
| **State Management**          | Manual `DownloadState` object passed through `AsyncState` parameter          | Shared `DownloadState` object captured by lambda closures                       | Local variables - compiler lifts to generated state machine class |
| **Control Flow Pattern**      | State machine with explicit transitions in callbacks                         | Functional composition - each continuation returns next task                    | Sequential imperative code - state machine generated by compiler  |
| **Task Completion Signaling** | `TaskCompletionSource<DownloadResult>` set in final callback                 | Final continuation returns `DownloadResult`                                     | `return` statement in async method                                |
| **Error Propagation**         | Try-catch in each callback, sets `Tcs.SetResult()` with error                | `IsFaulted` checks in continuations, uses `SetException()`                      | Standard try-catch blocks, automatically propagated               |
| **Recursive Receive Loop**    | `ReceiveCallback` calls `BeginReceive` again until `bytesReceived == 0`      | `ReceiveAllAsync` returns continuation that recursively calls itself            | `while(true)` loop with `await ReceiveAsync()` inside             |
| **Continuation Context**      | Callbacks execute on thread pool threads                                     | `TaskContinuationOptions` control execution context                             | Default task scheduler, can use `ConfigureAwait()`                |
| **Lambda Usage**              | None - uses named callback methods                                           | Heavy use of lambdas in `.ContinueWith()`                                       | Only in task wrappers (`BeginSend` callbacks)                     |
| **Method Signatures**         | Synchronous methods returning `Task` (via `Tcs.Task`)                        | Synchronous methods returning `Task` and chained tasks                          | `async Task<T>` methods                                           |
| **Compiler Assistance**       | None - fully manual                                                          | None - manual task composition                                                  | Full - generates state machine, handles awaiter pattern           |
| **Task Unwrapping**           | Not needed - manual completion                                               | Required with `.Unwrap()` for nested tasks                                      | Automatic - `await Task<Task<T>>` returns `T`                     |
| **Variable Scope**            | State object fields shared across callbacks                                  | Lambda closures capture local variables and state                               | Method-level locals persisted across await points                 |

---

## Author Notes

This project is designed for educational purposes to understand asynchronous programming paradigms in C#. For production HTTP downloads, use `HttpClient` which provides a high-level, fully-featured API with connection pooling, automatic decompression, and proper HTTPS support.

The progression from callbacks → tasks with continuations → async/await mirrors the evolution of asynchronous programming in .NET and helps understand why modern approaches are preferred.
