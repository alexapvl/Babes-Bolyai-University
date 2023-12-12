from src.domain.book import Book
from src.repository.binary_file_repo import BinaryFileRepository
from unittest import TestCase

class TestBinary(TestCase):
    def test_add_book(self):
        binary_repository = BinaryFileRepository("../tests/test_binary.txt")
        book = Book("1234567890", "Aria Winter", "The Book") 
        binary_repository.add_book(book)
        self.assertEqual(len(binary_repository.books), 1)
        binary_repository.filter_books("The")  # to delete the book from the test file
