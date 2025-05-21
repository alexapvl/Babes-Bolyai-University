using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Backend.Models;
using Backend.Services;

namespace Backend.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class AuthController : ControllerBase
    {
        private readonly AuthService _authService;

        public AuthController(AuthService authService)
        {
            _authService = authService;
        }

        [HttpPost("register")]
        public async Task<ActionResult<AuthResponse>> Register(RegisterRequest request)
        {
            var result = await _authService.Register(request);
            if (!result.Success)
                return BadRequest(result);
            
            return Ok(result);
        }

        [HttpPost("login")]
        public async Task<ActionResult<AuthResponse>> Login(LoginRequest request)
        {
            var result = await _authService.Login(request);
            if (!result.Success)
                return Unauthorized(result);
            
            return Ok(result);
        }

        [Authorize]
        [HttpGet("validate")]
        public ActionResult ValidateToken()
        {
            return Ok(new { isValid = true });
        }

        [Authorize]
        [HttpGet("check-username/{username}")]
        public async Task<ActionResult> CheckUsernameExists(string username)
        {
            var exists = await _authService.UserExists(username);
            return Ok(new { exists });
        }
    }
} 