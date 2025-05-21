using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Mvc;
using Backend.Models;
using Backend.Services;
using System.Security.Claims;

namespace Backend.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    [Authorize]
    public class BooksController : ControllerBase
    {
        private readonly BookService _bookService;

        public BooksController(BookService bookService)
        {
            _bookService = bookService;
        }

        private int GetUserId()
        {
            var userIdClaim = User.FindFirst(ClaimTypes.NameIdentifier);
            if (userIdClaim == null || !int.TryParse(userIdClaim.Value, out int userId))
            {
                throw new UnauthorizedAccessException("Invalid user ID in token");
            }
            return userId;
        }

        [HttpGet]
        public async Task<ActionResult<IEnumerable<Book>>> GetBooks([FromQuery] string genre = "all", [FromQuery] bool allAuthors = false)
        {
            try
            {
                var userId = GetUserId();
                
                // Debug info
                Console.WriteLine($"GetBooks: genre={genre}, allAuthors={allAuthors}");
                
                var books = await _bookService.GetBooksAsync(userId, genre, allAuthors);
                return Ok(books);
            }
            catch (UnauthorizedAccessException ex)
            {
                return Unauthorized(new { message = ex.Message });
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { message = ex.Message });
            }
        }

        [HttpGet("{id}")]
        public async Task<ActionResult<Book>> GetBook(int id)
        {
            try
            {
                var userId = GetUserId();
                
                // First try to get any book by ID, regardless of ownership
                var book = await _bookService.GetBookByIdAsync(id);

                if (book == null)
                {
                    return NotFound(new { message = "Book not found" });
                }

                return Ok(book);
            }
            catch (UnauthorizedAccessException ex)
            {
                return Unauthorized(new { message = ex.Message });
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { message = ex.Message });
            }
        }

        [HttpPost]
        public async Task<ActionResult<Book>> AddBook(Book book)
        {
            try
            {
                book.UserId = GetUserId();
                var addedBook = await _bookService.AddBookAsync(book);
                return CreatedAtAction(nameof(GetBook), new { id = addedBook.Id }, addedBook);
            }
            catch (UnauthorizedAccessException ex)
            {
                return Unauthorized(new { message = ex.Message });
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { message = ex.Message });
            }
        }

        [HttpPut("{id}")]
        public async Task<ActionResult<Book>> UpdateBook(int id, Book book)
        {
            if (id != book.Id)
            {
                return BadRequest(new { message = "ID mismatch" });
            }

            try
            {
                var userId = GetUserId();
                var updatedBook = await _bookService.UpdateBookAsync(book, userId);

                if (updatedBook == null)
                {
                    return NotFound(new { message = "Book not found or you don't have permission to update it" });
                }

                return Ok(updatedBook);
            }
            catch (UnauthorizedAccessException ex)
            {
                return Unauthorized(new { message = ex.Message });
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { message = ex.Message });
            }
        }

        [HttpDelete("{id}")]
        public async Task<ActionResult> DeleteBook(int id)
        {
            try
            {
                var userId = GetUserId();
                var result = await _bookService.DeleteBookAsync(id, userId);

                if (!result)
                {
                    return NotFound(new { message = "Book not found or you don't have permission to delete it" });
                }

                return NoContent();
            }
            catch (UnauthorizedAccessException ex)
            {
                return Unauthorized(new { message = ex.Message });
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { message = ex.Message });
            }
        }

        [HttpPost("{id}/lend")]
        public async Task<ActionResult<Book>> LendBook(int id, [FromBody] LendRequest request)
        {
            try
            {
                var userId = GetUserId();
                var book = await _bookService.LendBookAsync(id, request.LentTo, request.LentDate, userId);

                if (book == null)
                {
                    return NotFound(new { message = "Book not found or you don't have permission to lend it" });
                }

                return Ok(book);
            }
            catch (UnauthorizedAccessException ex)
            {
                return Unauthorized(new { message = ex.Message });
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { message = ex.Message });
            }
        }

        [HttpPost("{id}/return")]
        public async Task<ActionResult<Book>> ReturnBook(int id)
        {
            try
            {
                var userId = GetUserId();
                var book = await _bookService.ReturnBookAsync(id, userId);

                if (book == null)
                {
                    return NotFound(new { message = "Book not found or you don't have permission to return it" });
                }

                return Ok(book);
            }
            catch (UnauthorizedAccessException ex)
            {
                return Unauthorized(new { message = ex.Message });
            }
            catch (Exception ex)
            {
                return StatusCode(500, new { message = ex.Message });
            }
        }
    }

    public class LendRequest
    {
        public string LentTo { get; set; } = string.Empty;
        public DateTime LentDate { get; set; }
    }
} 