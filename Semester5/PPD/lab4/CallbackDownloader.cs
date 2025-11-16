using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace HttpDownloader;

/// <summary>
/// Implementation 1: Event-driven callback approach
/// Uses BeginConnect/EndConnect, BeginSend/EndSend, BeginReceive/EndReceive directly
/// with callbacks handling state transitions
/// </summary>
public class CallbackDownloader
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
        public TaskCompletionSource<DownloadResult> Tcs { get; set; } = new TaskCompletionSource<DownloadResult>();
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
            
            // Begin connect
            state.Socket.BeginConnect(endPoint, ConnectCallback, state);
        }
        catch (Exception ex)
        {
            state.Tcs.SetResult(new DownloadResult
            {
                Url = url,
                Success = false,
                ErrorMessage = ex.Message,
                Implementation = "Callback"
            });
        }
        
        return state.Tcs.Task;
    }

    private static void ConnectCallback(IAsyncResult ar)
    {
        var state = (DownloadState)ar.AsyncState!;
        
        try
        {
            state.Socket.EndConnect(ar);
            Console.WriteLine($"[Callback] Connected to {state.Host}");
            
            // Build HTTP request
            var request = HttpParser.BuildHttpRequest(state.Host, state.Path);
            var requestBytes = Encoding.ASCII.GetBytes(request);
            
            // Begin send
            state.Socket.BeginSend(requestBytes, 0, requestBytes.Length, SocketFlags.None, SendCallback, state);
        }
        catch (Exception ex)
        {
            state.Socket.Close();
            state.Stopwatch.Stop();
            state.Tcs.SetResult(new DownloadResult
            {
                Url = state.Url,
                Success = false,
                ErrorMessage = $"Connect error: {ex.Message}",
                Duration = state.Stopwatch.Elapsed,
                Implementation = "Callback"
            });
        }
    }

    private static void SendCallback(IAsyncResult ar)
    {
        var state = (DownloadState)ar.AsyncState!;
        
        try
        {
            int bytesSent = state.Socket.EndSend(ar);
            Console.WriteLine($"[Callback] Sent {bytesSent} bytes to {state.Host}");
            
            // Begin receive
            state.Socket.BeginReceive(state.Buffer, 0, state.Buffer.Length, SocketFlags.None, ReceiveCallback, state);
        }
        catch (Exception ex)
        {
            state.Socket.Close();
            state.Stopwatch.Stop();
            state.Tcs.SetResult(new DownloadResult
            {
                Url = state.Url,
                Success = false,
                ErrorMessage = $"Send error: {ex.Message}",
                Duration = state.Stopwatch.Elapsed,
                Implementation = "Callback"
            });
        }
    }

    private static void ReceiveCallback(IAsyncResult ar)
    {
        var state = (DownloadState)ar.AsyncState!;
        
        try
        {
            int bytesReceived = state.Socket.EndReceive(ar);
            
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
                        
                        Console.WriteLine($"[Callback] Headers parsed for {state.Host}, Content-Length: {state.ContentLength}");
                    }
                }
                
                // Continue receiving
                state.Socket.BeginReceive(state.Buffer, 0, state.Buffer.Length, SocketFlags.None, ReceiveCallback, state);
            }
            else
            {
                // Connection closed
                state.Socket.Close();
                state.Stopwatch.Stop();
                
                int bodyLength = state.TotalBytesReceived - state.HeaderLength;
                
                Console.WriteLine($"[Callback] Download complete for {state.Host}, received {bodyLength} bytes");
                
                state.Tcs.SetResult(new DownloadResult
                {
                    Url = state.Url,
                    Success = true,
                    ContentLength = bodyLength,
                    Duration = state.Stopwatch.Elapsed,
                    Implementation = "Callback"
                });
            }
        }
        catch (Exception ex)
        {
            state.Socket.Close();
            state.Stopwatch.Stop();
            state.Tcs.SetResult(new DownloadResult
            {
                Url = state.Url,
                Success = false,
                ErrorMessage = $"Receive error: {ex.Message}",
                Duration = state.Stopwatch.Elapsed,
                Implementation = "Callback"
            });
        }
    }
}

