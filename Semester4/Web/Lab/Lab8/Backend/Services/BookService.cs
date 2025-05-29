using Microsoft.EntityFrameworkCore;
using Backend.Data;
using Backend.Models;

namespace Backend.Services
{
    public class BookService
    {
        private readonly LibraryContext _context;

        public BookService(LibraryContext context)
        {
            _context = context;
        }

        public async Task<IEnumerable<Book>> GetBooksAsync(int userId, string genre = "all", bool allUsers = false)
        {
            var query = _context.Books.AsQueryable();
            
            // If not requesting all users' books, filter by the current user's ID
            if (!allUsers)
            {
                query = query.Where(b => b.UserId == userId);
            }

            if (!string.IsNullOrEmpty(genre) && genre.ToLower() != "all")
            {
                query = query.Where(b => b.Genre == genre);
            }

            return await query.ToListAsync();
        }

        public async Task<Book?> GetBookByIdAsync(int id)
        {
            return await _context.Books.FirstOrDefaultAsync(b => b.Id == id);
        }

        public async Task<Book?> GetBookByIdForUserAsync(int id, int userId)
        {
            return await _context.Books.FirstOrDefaultAsync(b => b.Id == id && b.UserId == userId);
        }

        public async Task<Book> AddBookAsync(Book book)
        {
            _context.Books.Add(book);
            await _context.SaveChangesAsync();
            return book;
        }

        public async Task<Book?> UpdateBookAsync(Book book, int userId)
        {
            var existingBook = await _context.Books.FirstOrDefaultAsync(b => b.Id == book.Id && b.UserId == userId);
            if (existingBook == null)
                return null;

            existingBook.Title = book.Title;
            existingBook.Author = book.Author;
            existingBook.Genre = book.Genre;
            existingBook.Pages = book.Pages;
            existingBook.LentTo = book.LentTo;
            existingBook.LentDate = book.LentDate;

            await _context.SaveChangesAsync();
            return existingBook;
        }

        public async Task<bool> DeleteBookAsync(int id, int userId)
        {
            var book = await _context.Books.FirstOrDefaultAsync(b => b.Id == id && b.UserId == userId);
            if (book == null)
                return false;

            _context.Books.Remove(book);
            await _context.SaveChangesAsync();
            return true;
        }

        public async Task<Book?> LendBookAsync(int id, string lentTo, DateTime lentDate, int userId)
        {
            var book = await _context.Books.FirstOrDefaultAsync(b => b.Id == id && b.UserId == userId);
            if (book == null)
                return null;

            book.LentTo = lentTo;
            book.LentDate = lentDate;

            await _context.SaveChangesAsync();
            return book;
        }

        public async Task<Book?> ReturnBookAsync(int id, int userId)
        {
            var book = await _context.Books.FirstOrDefaultAsync(b => b.Id == id && b.UserId == userId);
            if (book == null)
                return null;

            book.LentTo = null;
            book.LentDate = null;

            await _context.SaveChangesAsync();
            return book;
        }
    }
} 