from src.domain.book import Book
from src.repository.memory_repo import MemoryRepository
import json

class JsonRepository(MemoryRepository):
    def __init__(self, file_path: str):
        super().__init__()
        self.file_path = file_path

    def load_repo(self):
        with open(self.file_path, 'r') as file:
            data = json.load(file)["books"]
            self.books = [JsonRepository.unformat_book(book) for book in data]

    # append to file not possible in .json files, so it is not implemented, when adding a book, the whole file is overwritten

    def overwrite_repo(self):
        with open(self.file_path, 'w') as file:
            data = {"books": [JsonRepository.format_book(book) for book in self.books]}
            json.dump(data, file)

    def add_book(self, book: Book):
        """
        Add a book to the library by appending it to the list of books.
        We use the super() method inherited from MemoryRepository to add the book to the list of books and to
        update the history of the library.
        After that, the whole file is overwritten with the new list of books because appending to a .json file is not possible.
        (as far as I know -_-)
        :param book: the book to be added
        """
        super().add_book(book)
        self.overwrite_repo()

    def filter_books(self, keyword: str):
        super().filter_books(keyword)
        self.overwrite_repo()

    def undo(self):
        super().undo()
        self.overwrite_repo()

    @property
    def file_path(self):
        return self.__file_path

    @file_path.setter
    def file_path(self, file_path):
        self.__file_path = file_path

    @classmethod
    def format_book(cls, book: Book):
        return {"isbn": book.isbn, "author": book.author, "title": book.title}

    @classmethod
    def unformat_book(cls, data: dict):
        isbn = data["isbn"]
        author = data["author"]
        title = data["title"]
        return Book(isbn, author, title)
