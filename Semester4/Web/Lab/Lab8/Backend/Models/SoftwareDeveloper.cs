namespace Backend.Models
{
    public class SoftwareDeveloper
    {
        public int id { get; set; }
        public string name { get; set; } = string.Empty;
        public int? age { get; set; }
        public string? skills { get; set; } = string.Empty;
    }
} 