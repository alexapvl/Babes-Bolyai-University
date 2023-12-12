from src.domain.book import Book
from src.repository.memory_repo import MemoryRepository
from src.repository.text_file_repo import TextFileRepository
from src.repository.binary_file_repo import BinaryFileRepository
from src.repository.database_repo import DatabaseRepository
from src.repository.json_repo import JsonRepository
from src.services.book_service import BookService
from colorama import Fore, Style
import configparser

class UI:
    def __init__(self, book_service: BookService):
        self.book_service = book_service

    def run(self):
        while True:
            try:
                UI.menu()
                user_input = input(">>> ")
                if user_input == "1":
                    self.add_book()
                elif user_input == "2":
                    self.display_books()
                elif user_input == "3":
                    self.filter_books()
                elif user_input == "4":
                    self.undo()
                elif user_input == "0":
                    print(Fore.MAGENTA + "Bye!" + Style.RESET_ALL)
                    break
                else:
                    raise ValueError(Fore.RED + "Invalid option" + Style.RESET_ALL)
            except ValueError as ve:
                print(ve)

    def add_book(self):
        isbn = input(Fore.YELLOW + "ISBN: " + Style.RESET_ALL)
        author = input(Fore.YELLOW + "Author: " + Style.RESET_ALL)
        title = input(Fore.YELLOW + "Title: " + Style.RESET_ALL)
        book = Book(isbn, author, title)
        self.book_service.add_book(book)

    def display_books(self):
        for book in self.book_service.display_books():
            print(book)

    def filter_books(self):
        keyword = input(Fore.YELLOW + "Filter keyword:" + Style.RESET_ALL)
        self.book_service.filter_books(keyword)

    def undo(self):
        self.book_service.undo()
        print(Fore.GREEN + "Undo successful" + Style.RESET_ALL)

    @classmethod
    def get_config(cls):
        config = configparser.ConfigParser()
        config.read("../settings/settings.properties")
        repo_type = config.get('SettingsSection', 'settings.user')
        return repo_type

    @classmethod
    def menu(cls):
        print("1. Add a book")
        print("2. Display the list of books")
        print("3. Filter book list by first word of title")
        print("4. Undo last operation")
        print("0. Exit")

if __name__ == "__main__":
    repo_type = UI.get_config()
    if repo_type == "Memory":
        print(Fore.MAGENTA + "Welcome to the Memory Repository!" + Style.RESET_ALL)
        book_repo = MemoryRepository()
        book_repo.add_10_random_books()
    elif repo_type == "Text":
        print(Fore.MAGENTA + "Welcome to the Text File Repository!" + Style.RESET_ALL)
        book_repo = TextFileRepository("../repository/text_file.txt")
        book_repo.load_repo()
    elif repo_type == "Binary":
        print(Fore.MAGENTA + "Welcome to the Binary File Repository!" + Style.RESET_ALL)
        book_repo = BinaryFileRepository("../repository/binary_file.txt")
        book_repo.load_repo()
    elif repo_type == "Database":
        print(Fore.MAGENTA + "Welcome to the Database Repository!" + Style.RESET_ALL)
        book_repo = DatabaseRepository("../repository/database.db")
        book_repo.load_repo()
    elif repo_type == "Json":
        print(Fore.MAGENTA + "Welcome to the Json Repository!" + Style.RESET_ALL)
        book_repo = JsonRepository("../repository/json_repo.json")
        book_repo.load_repo()
    else:
        raise ValueError(Fore.RED + "Invalid setting in settings.properties file!\nSetting should be one of the following: 'Memory', 'Text', 'Binary' or 'Database'" + Style.RESET_ALL)
    book_service = BookService(book_repo)
    ui = UI(book_service)
    ui.run()
