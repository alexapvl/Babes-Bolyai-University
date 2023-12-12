import pickle
from src.repository.memory_repo import MemoryRepository
from src.domain.book import Book

class BinaryFileRepository(MemoryRepository):
    def __init__(self, file_path):
        self.file_path = file_path
        super().__init__()

    def load_repo(self):
        with open(self.file_path, "rb") as file:
            self.books = pickle.load(file)  # list of books

    def append_to_repo(self, book):
        with open(self.file_path, 'ab') as file:
            pickle.dump(book, file)

    def overwrite_repo(self):
        with open(self.file_path, 'wb') as file:
            pickle.dump(self.books, file)

    def add_book(self, book: Book):
        """
        Add a book to the library by appending it to the list of books.
        We use the super() method inherited from MemoryRepository to add the book to the list of books and to
        update the history of the library.
        After appending, the book is appended to the binary file.
        :param book: the book to be added
        """
        super().add_book(book)
        self.append_to_repo(book)

    def filter_books(self, keyword: str):
        super().filter_books(keyword)
        self.overwrite_repo()

    def undo(self):
        super().undo()
        self.overwrite_repo()

    @property  # getter
    def file_path(self):
        return self.__file_path

    @file_path.setter
    def file_path(self, file_path):
        self.__file_path = file_path
