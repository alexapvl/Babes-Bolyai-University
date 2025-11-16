using HttpDownloader;

class Program
{
    static async Task Main(string[] args)
    {
        Console.WriteLine("HTTP Downloader - Three Implementations Demo");
        Console.WriteLine("==============================================\n");
        
        // Test URLs - using HTTP (not HTTPS) for simplicity with raw sockets
        var urls = new[]
        {
            "http://www.example.com/",
            "http://www.example.org/",
            "http://www.example.net/"
        };
        
        Console.WriteLine("Test URLs:");
        foreach (var url in urls)
        {
            Console.WriteLine($"  - {url}");
        }
        Console.WriteLine();
        
        // Implementation 1: Callback-based (Event-driven)
        Console.WriteLine("\n========================================");
        Console.WriteLine("Implementation 1: Callback-based");
        Console.WriteLine("========================================");
        await RunImplementation("Callback", urls, CallbackDownloader.DownloadAsync);
        
        // Implementation 2: Task-based with ContinueWith()
        Console.WriteLine("\n========================================");
        Console.WriteLine("Implementation 2: Task-based (ContinueWith)");
        Console.WriteLine("========================================");
        await RunImplementation("Task", urls, TaskDownloader.DownloadAsync);
        
        // Implementation 3: Async/Await
        Console.WriteLine("\n========================================");
        Console.WriteLine("Implementation 3: Async/Await");
        Console.WriteLine("========================================");
        await RunImplementation("Async/Await", urls, AsyncAwaitDownloader.DownloadAsync);
        
        Console.WriteLine("\n========================================");
        Console.WriteLine("All downloads complete!");
        Console.WriteLine("========================================");
    }
    
    static async Task RunImplementation(
        string name, 
        string[] urls, 
        Func<string, Task<DownloadResult>> downloader)
    {
        var startTime = DateTime.Now;
        Console.WriteLine($"Starting {name} downloads at {startTime:HH:mm:ss.fff}");
        
        // Start all downloads simultaneously
        var tasks = urls.Select(url => downloader(url)).ToArray();
        
        // Wait for all to complete (this is the only Wait allowed in Main)
        var results = await Task.WhenAll(tasks);
        
        var endTime = DateTime.Now;
        var totalDuration = endTime - startTime;
        
        // Display results
        Console.WriteLine($"\n{name} Results:");
        Console.WriteLine("----------------------------------------");
        
        foreach (var result in results)
        {
            if (result.Success)
            {
                Console.WriteLine($"✓ {result.Url}");
                Console.WriteLine($"  Downloaded: {result.ContentLength} bytes");
                Console.WriteLine($"  Duration: {result.Duration.TotalMilliseconds:F2} ms");
            }
            else
            {
                Console.WriteLine($"✗ {result.Url}");
                Console.WriteLine($"  Error: {result.ErrorMessage}");
                Console.WriteLine($"  Duration: {result.Duration.TotalMilliseconds:F2} ms");
            }
        }
        
        Console.WriteLine($"\nTotal execution time: {totalDuration.TotalMilliseconds:F2} ms");
        Console.WriteLine($"Successful downloads: {results.Count(r => r.Success)}/{results.Length}");
    }
}

