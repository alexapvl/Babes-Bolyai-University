from src.domain.book import Book
from src.repository.memory_repo import MemoryRepository
from unittest import TestCase

class TestMemory(TestCase):
    def test_add_book(self):
        memory_repository = MemoryRepository()
        book = Book("1234567890", "Aria Winter", "The Book") 
        memory_repository.add_book(book)
        self.assertEqual(len(memory_repository.books), 1)
        