using System.Text;

namespace HttpDownloader;

public class HttpParser
{
    public static (Dictionary<string, string> headers, int headerLength) ParseHeaders(string response)
    {
        var headers = new Dictionary<string, string>(StringComparer.OrdinalIgnoreCase);
        var lines = response.Split(new[] { "\r\n" }, StringSplitOptions.None);
        
        int headerEndIndex = 0;
        bool foundEmptyLine = false;
        
        for (int i = 1; i < lines.Length; i++) // Skip first line (status line)
        {
            if (string.IsNullOrWhiteSpace(lines[i]))
            {
                headerEndIndex = i + 1;
                foundEmptyLine = true;
                break;
            }
            
            var colonIndex = lines[i].IndexOf(':');
            if (colonIndex > 0)
            {
                var key = lines[i].Substring(0, colonIndex).Trim();
                var value = lines[i].Substring(colonIndex + 1).Trim();
                headers[key] = value;
            }
        }
        
        if (!foundEmptyLine)
        {
            return (headers, -1); // Headers incomplete
        }
        
        // Calculate header length in bytes
        int headerLength = 0;
        for (int i = 0; i < headerEndIndex; i++)
        {
            headerLength += Encoding.ASCII.GetByteCount(lines[i]) + 2; // +2 for \r\n
        }
        
        return (headers, headerLength);
    }
    
    public static int GetContentLength(Dictionary<string, string> headers)
    {
        if (headers.TryGetValue("Content-Length", out var contentLengthStr) &&
            int.TryParse(contentLengthStr, out var contentLength))
        {
            return contentLength;
        }
        return -1; // Unknown content length
    }
    
    public static string BuildHttpRequest(string host, string path)
    {
        var request = new StringBuilder();
        request.Append($"GET {path} HTTP/1.1\r\n");
        request.Append($"Host: {host}\r\n");
        request.Append("User-Agent: SimpleHttpDownloader/1.0\r\n");
        request.Append("Connection: close\r\n");
        request.Append("\r\n");
        return request.ToString();
    }
    
    public static (string host, string path, int port) ParseUrl(string url)
    {
        // Remove http:// or https:// prefix
        var uri = new Uri(url);
        var host = uri.Host;
        var path = uri.PathAndQuery;
        var port = uri.Port;
        
        if (path == "")
            path = "/";
            
        return (host, path, port);
    }
}

public class DownloadResult
{
    public string Url { get; set; } = "";
    public bool Success { get; set; }
    public int ContentLength { get; set; }
    public string ErrorMessage { get; set; } = "";
    public TimeSpan Duration { get; set; }
    public string Implementation { get; set; } = "";
}

