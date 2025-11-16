using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace HttpDownloader;

/// <summary>
/// Implementation 2: Task-based with ContinueWith()
/// Wraps BeginConnect/EndConnect, BeginSend/EndSend, BeginReceive/EndReceive in Tasks
/// and chains them using ContinueWith()
/// </summary>
public class TaskDownloader
{
    private class DownloadState
    {
        public Socket Socket { get; set; } = null!;
        public string Url { get; set; } = "";
        public string Host { get; set; } = "";
        public string Path { get; set; } = "";
        public byte[] Buffer { get; set; } = new byte[8192];
        public StringBuilder ResponseData { get; set; } = new StringBuilder();
        public Stopwatch Stopwatch { get; set; } = new Stopwatch();
        public bool HeadersParsed { get; set; } = false;
        public int ContentLength { get; set; } = -1;
        public int HeaderLength { get; set; } = 0;
        public int TotalBytesReceived { get; set; } = 0;
    }

    public static Task<DownloadResult> DownloadAsync(string url)
    {
        var state = new DownloadState { Url = url };
        state.Stopwatch.Start();
        
        try
        {
            var (host, path, port) = HttpParser.ParseUrl(url);
            state.Host = host;
            state.Path = path;
            
            // Resolve DNS
            var addresses = Dns.GetHostAddresses(host);
            var endPoint = new IPEndPoint(addresses[0], port);
            
            // Create socket
            state.Socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
            
            // Chain operations using ContinueWith
            return ConnectAsync(state.Socket, endPoint)
                .ContinueWith(connectTask =>
                {
                    if (connectTask.IsFaulted)
                        throw connectTask.Exception!;
                    
                    Console.WriteLine($"[Task] Connected to {state.Host}");
                    
                    // Build HTTP request
                    var request = HttpParser.BuildHttpRequest(state.Host, state.Path);
                    var requestBytes = Encoding.ASCII.GetBytes(request);
                    
                    return SendAsync(state.Socket, requestBytes);
                }, TaskContinuationOptions.OnlyOnRanToCompletion)
                .Unwrap()
                .ContinueWith(sendTask =>
                {
                    if (sendTask.IsFaulted)
                        throw sendTask.Exception!;
                    
                    Console.WriteLine($"[Task] Sent {sendTask.Result} bytes to {state.Host}");
                    
                    return ReceiveAllAsync(state);
                }, TaskContinuationOptions.OnlyOnRanToCompletion)
                .Unwrap()
                .ContinueWith(receiveTask =>
                {
                    state.Socket.Close();
                    state.Stopwatch.Stop();
                    
                    if (receiveTask.IsFaulted)
                    {
                        return new DownloadResult
                        {
                            Url = url,
                            Success = false,
                            ErrorMessage = receiveTask.Exception!.GetBaseException().Message,
                            Duration = state.Stopwatch.Elapsed,
                            Implementation = "Task"
                        };
                    }
                    
                    int bodyLength = state.TotalBytesReceived - state.HeaderLength;
                    Console.WriteLine($"[Task] Download complete for {state.Host}, received {bodyLength} bytes");
                    
                    return new DownloadResult
                    {
                        Url = url,
                        Success = true,
                        ContentLength = bodyLength,
                        Duration = state.Stopwatch.Elapsed,
                        Implementation = "Task"
                    };
                });
        }
        catch (Exception ex)
        {
            state.Stopwatch.Stop();
            return Task.FromResult(new DownloadResult
            {
                Url = url,
                Success = false,
                ErrorMessage = ex.Message,
                Duration = state.Stopwatch.Elapsed,
                Implementation = "Task"
            });
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

    private static Task ReceiveAllAsync(DownloadState state)
    {
        return ReceiveAsync(state.Socket, state.Buffer)
            .ContinueWith(receiveTask =>
            {
                if (receiveTask.IsFaulted)
                    throw receiveTask.Exception!;
                
                int bytesReceived = receiveTask.Result;
                
                if (bytesReceived > 0)
                {
                    state.TotalBytesReceived += bytesReceived;
                    var chunk = Encoding.ASCII.GetString(state.Buffer, 0, bytesReceived);
                    state.ResponseData.Append(chunk);
                    
                    // Parse headers if not done yet
                    if (!state.HeadersParsed)
                    {
                        var response = state.ResponseData.ToString();
                        if (response.Contains("\r\n\r\n"))
                        {
                            var (headers, headerLength) = HttpParser.ParseHeaders(response);
                            state.HeadersParsed = true;
                            state.HeaderLength = headerLength;
                            state.ContentLength = HttpParser.GetContentLength(headers);
                            
                            Console.WriteLine($"[Task] Headers parsed for {state.Host}, Content-Length: {state.ContentLength}");
                        }
                    }
                    
                    // Continue receiving
                    return ReceiveAllAsync(state);
                }
                else
                {
                    // Connection closed
                    return Task.CompletedTask;
                }
            }, TaskContinuationOptions.OnlyOnRanToCompletion)
            .Unwrap();
    }
}

