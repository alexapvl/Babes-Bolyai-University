from random import shuffle, choice
from colorama import Fore, Style

class Book:
    def __init__(self, isbn: str, author: str, title: str):
        self.isbn = isbn
        self.author = author
        self.title = title

    @property  # getter
    def isbn(self):
        return self.__isbn

    @isbn.setter
    def isbn(self, isbn):
        """
        Before setting the isbn, the isbn needs to be validated and needs to respect
        the following:
            1. Formed only with digits
            2. Length is 10 digits
            3. All digits are unique
        """
        if not isbn.isdigit() or len(set(isbn)) != 10:
            raise ValueError(Fore.RED + "Invalid ISBN" + Style.RESET_ALL)
        self.__isbn = isbn

    @property  # getter
    def author(self):
        return self.__author

    @author.setter
    def author(self, author):
        self.__author = author

    @property
    def title(self):
        return self.__title

    @title.setter
    def title(self, title):
        self.__title = title

    def __str__(self):
        return "ISBN:" + self.__isbn + " - '" + self.__title + "' by " + self.__author

    def __eq__(self, other):
        # Two books will be the same if they have the same isbn because the isbn needs to be unique
        # Or if they have the same author and title, because there cannot be more isbn(s) for the same book
        if not isinstance(other, Book):
            raise ValueError("Not a book.")
        return self.isbn == other.isbn or (self.title == other.title and self.author == other.author)

    @classmethod
    def random_book(cls):
        digits = "0123456789"
        digits_list = list(digits)
        shuffle(digits_list)
        isbn = ''.join(digits_list)
        authors = [
            "Aria Winter",
            "Sebastian Black",
            "Isabella Rivers",
            "Maxwell Sterling",
            "Elena Moon",
            "Gabriel Storm",
            "Olivia Raven",
            "Lucas Ember",
            "Sophia Nightshade",
            "Nathaniel Frost"
        ]
        book_titles = [
            "Whispers of the Celestial Sands",
            "Ephemeral Echoes",
            "Midnight Serenade",
            "The Enigmatic Cipher",
            "Labyrinth of Dreams",
            "Chronicles of the Nebula",
            "Silent Symphony",
            "Echoes of the Forgotten Realm",
            "Sapphire Shadows",
            "The Quantum Paradox"
        ]
        return Book(isbn, choice(authors), choice(book_titles))
