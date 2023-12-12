from src.domain.book import Book
from colorama import Fore, Style

class MemoryRepository:
    def __init__(self):
        self.books = []
        self.history = []

    def add_10_random_books(self):
        while len(self.books) < 10:
            random_book = Book.random_book()
            if random_book not in self.__books:
                self.books.append(random_book)

    def add_book(self, book: Book):
        """
        Add a book to the library by appending it to the list of books.
        After appending, the history of the library is updated by appending a tuple consisting of
        the operation and the book that was added. The operation is "add" in this case and will be used
        in case we want to undo the operation. The book is the book that was added and will be used to remove
        the book from the list of books in case we want to undo the operation. Since the book is unique, we can
        use the book itself to remove it from the list of books with the remove method corresponding to lists.
        :param book: the book to be added
        """
        self.books.append(book)
        self.history.append(("add", book))

    def display_books(self):
        return self.books

    def filter_books(self, keyword: str):
        filtered_library = [book for book in self.books if not book.title.startswith(keyword)]
        self.history.append(("filter", self.books.copy()))
        self.books = filtered_library

    def undo(self):
        if self.history:
            operation, data = self.history.pop()
            if operation == "add":
                self.books.remove(data)
            elif operation == "filter":
                self.books = data
        else:
            raise ValueError(Fore.RED + "Cannot undo anymore" + Style.RESET_ALL)

    @property  # getter
    def books(self):
        return self.__books

    @books.setter
    def books(self, books):
        self.__books = books

    @property  # getter
    def history(self):
        return self.__history

    @history.setter
    def history(self, history):
        self.__history = history
