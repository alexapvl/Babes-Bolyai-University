using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Design;
using Microsoft.Extensions.Configuration;

namespace Backend.Data
{
    public class ProjectContextFactory : IDesignTimeDbContextFactory<ProjectContext>
    {
        public ProjectContext CreateDbContext(string[] args)
        {
            var configuration = new ConfigurationBuilder()
                .SetBasePath(Directory.GetCurrentDirectory())
                .AddJsonFile("appsettings.json")
                .Build();

            var optionsBuilder = new DbContextOptionsBuilder<ProjectContext>();
            var connectionString = configuration.GetConnectionString("DefaultConnection");
            
            optionsBuilder.UseMySql(connectionString, ServerVersion.AutoDetect(connectionString));

            return new ProjectContext(optionsBuilder.Options);
        }
    }
} 