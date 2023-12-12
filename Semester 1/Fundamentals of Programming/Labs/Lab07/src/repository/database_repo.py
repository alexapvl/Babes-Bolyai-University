import sqlite3
from src.domain.book import Book
from src.repository.memory_repo import MemoryRepository

class DatabaseRepository(MemoryRepository):
    def __init__(self, database_path: str):
        super().__init__()
        self.database_path = database_path  # path to database
        self.__connection = sqlite3.connect(self.database_path)
        self.__cursor = self.connection.cursor()

    def load_repo(self):
        self.cursor.execute("SELECT * FROM books")
        for row in self.cursor.fetchall():
            self.books.append(DatabaseRepository.unformat_book(row))

    def append_to_repo(self, book):
        self.cursor.execute("INSERT INTO books VALUES (?, ?, ?)", (book.isbn, book.author, book.title))
        self.connection.commit()

    def overwrite_repo(self):
        self.cursor.execute("DELETE FROM books")
        for book in self.books:
            self.cursor.execute("INSERT INTO books VALUES (?, ?, ?)", (book.isbn, book.author, book.title))
        self.connection.commit()

    def add_book(self, book):
        """
        Add a book to the library by appending it to the list of books.
        We use the super() method inherited from MemoryRepository to add the book to the list of books and to
        update the history of the library.
        After appending, the book is appended to the database by insertion.
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
    def database_path(self):
        return self.__database_path

    @database_path.setter
    def database_path(self, database_path: str):
        self.__database_path = database_path

    @property # getter
    def connection(self):
        return self.__connection

    @property # getter
    def cursor(self):
        return self.__cursor

    @classmethod
    def format_book(cls, book):
        return book.isbn, book.author, book.title

    @classmethod
    def unformat_book(cls, row):
        return Book(row[0], row[1], row[2])
