from src.domain.book import Book
from src.repository.text_file_repo import TextFileRepository
from unittest import TestCase

class TestFile(TestCase):
    def test_add_book(self):
        file_repository = TextFileRepository("../tests/test_file.txt")
        book = Book("1234567890", "Aria Winter", "The Book") 
        file_repository.add_book(book)
        self.assertEqual(len(file_repository.books), 1)
        file_repository.filter_books("The")  # to delete the book from the test file
    