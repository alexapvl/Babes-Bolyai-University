class SnakeError(Exception):
    pass

class Snake:
    def __init__(self):
        self.head_x = 0
        self.head_y = 0

    @property
    def head_x(self):
        return self.__head_x

    @head_x.setter
    def head_x(self, x: int):
        self.__head_x = x

    @property
    def head_y(self):
        return self.__head_y

    @head_y.setter
    def head_y(self, y: int):
        self.__head_y = y
