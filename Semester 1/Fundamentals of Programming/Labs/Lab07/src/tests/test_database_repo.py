from src.domain.book import Book
from src.repository.database_repo import DatabaseRepository
from unittest import TestCase

class TestDataBase(TestCase):
    def test_add_book(self):
        database_repository = DatabaseRepository("../tests/test_database.db")
        book = Book("1234567890", "Aria Winter", "The Book") 
        database_repository.add_book(book)
        self.assertEqual(len(database_repository.books), 1)
        database_repository.filter_books("The")  # to delete the book from the test file
