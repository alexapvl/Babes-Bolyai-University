using Microsoft.EntityFrameworkCore;
using Backend.Models;

namespace Backend.Data
{
    public class ProjectContext : DbContext
    {
        public ProjectContext(DbContextOptions<ProjectContext> options) : base(options)
        {
        }

        public DbSet<SoftwareDeveloper> SoftwareDevelopers { get; set; }
        public DbSet<Project> Projects { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<SoftwareDeveloper>(entity =>
            {
                entity.HasKey(s => s.id);
                entity.Property(s => s.id).ValueGeneratedOnAdd();
            });

            modelBuilder.Entity<Project>(entity =>
            {
                entity.HasKey(p => p.id);
                entity.Property(p => p.id).ValueGeneratedOnAdd();
                entity.HasOne<SoftwareDeveloper>()
                    .WithMany()
                    .HasForeignKey(p => p.projectManagerId);
            });
        }
    }
} 