from src.domain.book import Book
from unittest import TestCase

class TestBook(TestCase):
    def test_book_isbn(self):
        book = Book("1234567890", "Aria Winter", "The Book")
        self.assertEqual(book.isbn, "1234567890")
    
    def test_book_author(self):
        book = Book("1234567890", "Aria Winter", "The Book")
        self.assertEqual(book.author, "Aria Winter")
    
    def test_book_title(self):
        book = Book("1234567890", "Aria Winter", "The Book")
        self.assertEqual(book.title, "The Book")
    
    def test_book_str(self):
        book = Book("1234567890", "Aria Winter", "The Book")
        self.assertEqual(str(book), "ISBN:1234567890 - 'The Book' by Aria Winter")
    
    def test_book_eq(self):
        book1 = Book("1234567890", "Aria Winter", "The Book")
        book2 = Book("1234567890", "Aria Winter", "The Book")
        self.assertEqual(book1, book2)
