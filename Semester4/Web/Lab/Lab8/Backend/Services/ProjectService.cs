using Microsoft.EntityFrameworkCore;
using Backend.Data;
using Backend.Models;

namespace Backend.Services
{
    public class ProjectService
    {
        private readonly ProjectContext _context;

        public ProjectService(ProjectContext context)
        {
            _context = context;
        }

        public async Task<IEnumerable<Project>> GetProjectsAsync(string? memberName = null)
        {
            var query = _context.Projects.AsQueryable();
            
            if (!string.IsNullOrEmpty(memberName))
            {
                query = query.Where(p => p.members.Contains(memberName));
            }

            return await query.ToListAsync();
        }

        public async Task<Project> AddProjectAsync(Project project)
        {
            _context.Projects.Add(project);
            await _context.SaveChangesAsync();
            return project;
        }

        public async Task<Project?> UpdateProjectAsync(Project project)
        {
            var existingProject = await _context.Projects.FirstOrDefaultAsync(p => p.id == project.id);
            if (existingProject == null)
                return null;

            existingProject.name = project.name;
            existingProject.description = project.description;
            existingProject.members = project.members;

            await _context.SaveChangesAsync();
            return existingProject;
        }
    }
} 