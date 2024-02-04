import texttable
from texttable import Texttable

class Board:
    def __init__(self, n: int, apples: int):
        self.size = n
        self.data = n
        self.apples = apples  # number of apples on the board

    def __str__(self):
        t = texttable.Texttable()
        for row in self.data:
            t.add_row(row)
        return t.draw()

    @property
    def size(self):
        return self.__size

    @size.setter
    def size(self, n: int):
        self.__size = n

    @property
    def data(self):
        return self.__data

    @data.setter
    def data(self, n: int):
        self.__data = [[" " for _ in range(n)] for _ in range(n)]

    @property
    def apples(self):
        return self.__apples

    @apples.setter
    def apples(self, apples):
        self.__apples = apples
