using Microsoft.EntityFrameworkCore;
using Backend.Data;
using Backend.Models;

namespace Backend.Services
{
    public class SoftwareDeveloperService
    {
        private readonly ProjectContext _context;

        public SoftwareDeveloperService(ProjectContext context)
        {
            _context = context;
        }

        public async Task<IEnumerable<SoftwareDeveloper>> GetDevelopersAsync()
        {
            var query = _context.SoftwareDevelopers.AsQueryable();

            return await query.ToListAsync();
        }

        public async Task<SoftwareDeveloper?> GetDeveloperByNameAsync(string name) {
          return await _context.SoftwareDevelopers.FirstOrDefaultAsync(s => s.name == name);
        }

        public async Task<SoftwareDeveloper> AddDeveloperAsync(SoftwareDeveloper developer)
        {
            _context.SoftwareDevelopers.Add(developer);
            await _context.SaveChangesAsync();
            return developer;
        }
    }
} 