from src.repository.memory_repo import MemoryRepository
from src.domain.book import Book

class TextFileRepository(MemoryRepository):
    def __init__(self, file_path: str):
        self.file_path = file_path
        super().__init__()

    def load_repo(self):
        with open(self.file_path, 'r') as file:
            lines = file.readlines()  # List of strings
            self.books = [TextFileRepository.unformat_book(line) for line in lines]

    def append_to_repo(self, book):
        with open(self.file_path, 'a') as file:
            file.write(TextFileRepository.format_book(book))
            file.write("\n")

    def overwrite_repo(self):
        with open(self.file_path, 'w') as file:
            for book in self.books:
                file.write(self.format_book(book))
                file.write("\n")

    def add_book(self, book: Book):
        """
        Add a book to the library by appending it to the list of books.
        We use the super() method inherited from MemoryRepository to add the book to the list of books and to
        update the history of the library.
        After appending, the book is appended to the text file.
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

    @classmethod
    def format_book(cls, book: Book):
        return f"{book.isbn},{book.title},{book.author}"

    @classmethod
    def unformat_book(cls, line: str):
        isbn, title, author = line.strip().split(',')
        return Book(isbn, author, title)
