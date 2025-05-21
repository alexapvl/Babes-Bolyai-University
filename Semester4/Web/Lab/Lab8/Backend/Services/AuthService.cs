using System.IdentityModel.Tokens.Jwt;
using System.Security.Claims;
using System.Text;
using Microsoft.EntityFrameworkCore;
using Microsoft.IdentityModel.Tokens;
using Backend.Data;
using Backend.Models;

namespace Backend.Services
{
    public class AuthService
    {
        private readonly LibraryContext _context;
        private readonly IConfiguration _configuration;

        public AuthService(LibraryContext context, IConfiguration configuration)
        {
            _context = context;
            _configuration = configuration;
        }

        public async Task<AuthResponse> Register(RegisterRequest model)
        {
            // Check if user exists
            if (await _context.Users.AnyAsync(u => u.Username == model.Username))
            {
                return new AuthResponse
                {
                    Success = false,
                    Message = "Username already exists"
                };
            }

            // Create new user
            var user = new User
            {
                Username = model.Username,
                PasswordHash = BCrypt.Net.BCrypt.HashPassword(model.Password),
                CreatedAt = DateTime.UtcNow
            };

            _context.Users.Add(user);
            await _context.SaveChangesAsync();

            // Generate token
            var token = GenerateJwtToken(user);

            return new AuthResponse
            {
                Success = true,
                Message = "Registration successful",
                UserId = user.Id,
                Username = user.Username,
                Token = token
            };
        }

        public async Task<AuthResponse> Login(LoginRequest model)
        {
            // Find user by username
            var user = await _context.Users.FirstOrDefaultAsync(u => u.Username == model.Username);

            // Check if user exists and password is correct
            if (user == null || !BCrypt.Net.BCrypt.Verify(model.Password, user.PasswordHash))
            {
                return new AuthResponse
                {
                    Success = false,
                    Message = "Invalid username or password"
                };
            }

            // Update last login
            user.LastLogin = DateTime.UtcNow;
            await _context.SaveChangesAsync();

            // Generate token
            var token = GenerateJwtToken(user);

            return new AuthResponse
            {
                Success = true,
                Message = "Login successful",
                UserId = user.Id,
                Username = user.Username,
                Token = token
            };
        }

        // Method to check if a username exists
        public async Task<bool> UserExists(string username)
        {
            return await _context.Users.AnyAsync(u => u.Username == username);
        }

        private string GenerateJwtToken(User user)
        {
            var securityKey = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(_configuration["Jwt:Key"]));
            var credentials = new SigningCredentials(securityKey, SecurityAlgorithms.HmacSha256);

            var claims = new[]
            {
                new Claim(ClaimTypes.NameIdentifier, user.Id.ToString()),
                new Claim(ClaimTypes.Name, user.Username)
            };

            var token = new JwtSecurityToken(
                issuer: _configuration["Jwt:Issuer"],
                audience: _configuration["Jwt:Audience"],
                claims: claims,
                expires: DateTime.Now.AddMinutes(Convert.ToDouble(_configuration["Jwt:ExpiryInMinutes"])),
                signingCredentials: credentials
            );

            return new JwtSecurityTokenHandler().WriteToken(token);
        }
    }
}