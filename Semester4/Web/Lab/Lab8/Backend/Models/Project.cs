namespace Backend.Models
{
    public class Project
    {
        public int id { get; set; }
        public int projectManagerId { get; set; }
        public string name { get; set; } = string.Empty;
        public string? description { get; set; } = string.Empty;
        public string? members { get; set; } = string.Empty;
    }
} 