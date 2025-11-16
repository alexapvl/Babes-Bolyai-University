using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace HttpDownloader;

/// <summary>
/// Implementation 3: Async/Await approach
/// Wraps BeginConnect/EndConnect, BeginSend/EndSend, BeginReceive/EndReceive in Tasks
/// and uses async/await for clean, sequential-looking asynchronous code
/// </summary>
public class AsyncAwaitDownloader
{
    public static async Task<DownloadResult> DownloadAsync(string url)
    {
        var stopwatch = Stopwatch.StartNew();
        Socket? socket = null;
        
        try
        {
            var (host, path, port) = HttpParser.ParseUrl(url);
            
            // Resolve DNS
            var addresses = await Dns.GetHostAddressesAsync(host);
            var endPoint = new IPEndPoint(addresses[0], port);
            
            // Create socket
            socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            
            // Connect
            await ConnectAsync(socket, endPoint);
            Console.WriteLine($"[Async/Await] Connected to {host}");
            
            // Build and send HTTP request
            var request = HttpParser.BuildHttpRequest(host, path);
            var requestBytes = Encoding.ASCII.GetBytes(request);
            int bytesSent = await SendAsync(socket, requestBytes);
            Console.WriteLine($"[Async/Await] Sent {bytesSent} bytes to {host}");
            
            // Receive response
            var (responseData, totalBytes) = await ReceiveAllAsync(socket, host);
            
            // Parse headers
            var (headers, headerLength) = HttpParser.ParseHeaders(responseData.ToString());
            var contentLength = HttpParser.GetContentLength(headers);
            int bodyLength = totalBytes - headerLength;
            
            Console.WriteLine($"[Async/Await] Download complete for {host}, received {bodyLength} bytes");
            
            stopwatch.Stop();
            return new DownloadResult
            {
                Url = url,
                Success = true,
                ContentLength = bodyLength,
                Duration = stopwatch.Elapsed,
                Implementation = "Async/Await"
            };
        }
        catch (Exception ex)
        {
            stopwatch.Stop();
            return new DownloadResult
            {
                Url = url,
                Success = false,
                ErrorMessage = ex.Message,
                Duration = stopwatch.Elapsed,
                Implementation = "Async/Await"
            };
        }
        finally
        {
            socket?.Close();
        }
    }

    private static Task ConnectAsync(Socket socket, EndPoint endPoint)
    {
        var tcs = new TaskCompletionSource<bool>();
        
        socket.BeginConnect(endPoint, ar =>
        {
            try
            {
                socket.EndConnect(ar);
                tcs.SetResult(true);
            }
            catch (Exception ex)
            {
                tcs.SetException(ex);
            }
        }, null);
        
        return tcs.Task;
    }

    private static Task<int> SendAsync(Socket socket, byte[] buffer)
    {
        var tcs = new TaskCompletionSource<int>();
        
        socket.BeginSend(buffer, 0, buffer.Length, SocketFlags.None, ar =>
        {
            try
            {
                int bytesSent = socket.EndSend(ar);
                tcs.SetResult(bytesSent);
            }
            catch (Exception ex)
            {
                tcs.SetException(ex);
            }
        }, null);
        
        return tcs.Task;
    }

    private static Task<int> ReceiveAsync(Socket socket, byte[] buffer)
    {
        var tcs = new TaskCompletionSource<int>();
        
        socket.BeginReceive(buffer, 0, buffer.Length, SocketFlags.None, ar =>
        {
            try
            {
                int bytesReceived = socket.EndReceive(ar);
                tcs.SetResult(bytesReceived);
            }
            catch (Exception ex)
            {
                tcs.SetException(ex);
            }
        }, null);
        
        return tcs.Task;
    }

    private static async Task<(StringBuilder responseData, int totalBytes)> ReceiveAllAsync(Socket socket, string host)
    {
        var responseData = new StringBuilder();
        var buffer = new byte[8192];
        int totalBytesReceived = 0;
        bool headersParsed = false;
        
        while (true)
        {
            int bytesReceived = await ReceiveAsync(socket, buffer);
            
            if (bytesReceived == 0)
            {
                // Connection closed
                break;
            }
            
            totalBytesReceived += bytesReceived;
            var chunk = Encoding.ASCII.GetString(buffer, 0, bytesReceived);
            responseData.Append(chunk);
            
            // Parse headers on first occurrence
            if (!headersParsed && responseData.ToString().Contains("\r\n\r\n"))
            {
                var (headers, headerLength) = HttpParser.ParseHeaders(responseData.ToString());
                var contentLength = HttpParser.GetContentLength(headers);
                headersParsed = true;
                
                Console.WriteLine($"[Async/Await] Headers parsed for {host}, Content-Length: {contentLength}");
            }
        }
        
        return (responseData, totalBytesReceived);
    }
}

