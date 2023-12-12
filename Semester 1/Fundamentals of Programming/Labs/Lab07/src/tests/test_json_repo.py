from src.domain.book import Book
from src.repository.json_repo import JsonRepository
from unittest import TestCase

class TestJson(TestCase):
    def test_add_book(self):
        json_repository = JsonRepository("../tests/test_json.json")
        book = Book("1234567890", "Aria Winter", "The Book")
        json_repository.add_book(book)
        self.assertEqual(len(json_repository.books), 1)
        json_repository.filter_books("The")  # to delete the book from the test file
