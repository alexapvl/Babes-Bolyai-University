from src.domain.book import Book
from src.repository.memory_repo import MemoryRepository
from colorama import Fore, Style

class BookService:
    def __init__(self, book_repo: MemoryRepository):
        self.book_repository = book_repo

    def add_book(self, book: Book):
        """
        Add a book to the library by appending it to the list of books in the repository.
        First, we check if the book already exists in the repository.
        If it does, we raise a ValueError.
        If it doesn't, we add the book to the repository.
        We do this so we have only unique books in our repos.
        :param book: the book to be added
        """
        if book in self.book_repository.display_books():
            raise ValueError(Fore.RED + "Book already exists" + Style.RESET_ALL)
        self.book_repository.add_book(book)

    def display_books(self):
        return self.book_repository.display_books()

    def filter_books(self, keyword: str):
        self.book_repository.filter_books(keyword)

    def undo(self):
        self.book_repository.undo()
