from src.domain.board import Board
from src.domain.snake import Snake, SnakeError
from random import randint

class GameOver(Exception):
    pass

class Service:
    def __init__(self, board: Board, snake: Snake):
        self.board = board
        self.snake = snake
        self.moves = ["up", "up"]
        self.was_apple = False
        self.__opposite_directions = {"up": "down", "down": "up", "left": "right", "right": "left"}
        self.place_snake()  # when initializing the board place the snake
        self.place_apples(self.board.apples)  # place random apples

    @staticmethod
    def get_settings() -> tuple:
        with open("settings/settings.properties", "r") as settings:
            s = settings.readline()
            params = s.split()
        size = int(params[0])
        apples = int(params[1])
        return size, apples

    def place_snake(self):
        """
        Places the snake on the board
        """
        center = self.board.size // 2
        self.board.data[center][center] = "+"
        self.board.data[center + 1][center] = "+"
        self.board.data[center - 1][center] = "*"  # snake head
        self.snake.head_x = center - 1
        self.snake.head_y = center

    def place_apples(self, apples):
        """
        Places random apples on the board
        """
        no_apples = 0
        x = randint(0, self.board.size - 1)
        y = randint(0, self.board.size - 1)
        while no_apples < apples:
            if self.board.data[x][y] == " " and not self.is_adjacent(x, y):
                self.board.data[x][y] = "a"
                no_apples += 1
            x = randint(0, self.board.size - 1)
            y = randint(0, self.board.size - 1)

    def is_adjacent(self, x: int, y: int):
        if x == 0:
            if y == 0:
                if self.board.data[x][y + 1] == "a" or self.board.data[x + 1][y] == "a":
                    return True
            elif y == self.board.size - 1:
                if self.board.data[x][y - 1] == "a" or self.board.data[x + 1][y] == "a":
                    return True
            else:
                if self.board.data[x + 1][y] == "a" or self.board.data[x][y + 1] == "a" or self.board.data[x][y - 1] == "a":
                    return True
        elif x == self.board.size - 1:
            if y == 0:
                if self.board.data[x - 1][y] == "a" or self.board.data[x][y + 1] == "a":
                    return True
            elif y == self.board.size - 1:
                if self.board.data[x - 1][y] == "a" or self.board.data[x][y - 1] == "a":
                    return True
            else:
                if self.board.data[x - 1][y] == "a" or self.board.data[x][y + 1] == "a" or self.board.data[x][y - 1] == "a":
                    return True
        elif y == 0:
            if self.board.data[x][y + 1] == "a" or self.board.data[x - 1][y] == "a" or self.board.data[x + 1][y] == "a":
                return True
        elif y == self.board.size - 1:
            if self.board.data[x][y - 1] == "a" or self.board.data[x - 1][y] == "a" or self.board.data[x + 1][y] == "a":
                return True
        else:
            if self.board.data[x - 1][y] == "a" or self.board.data[x][y + 1] == "a" or self.board.data[x + 1][y] == "a" or self.board.data[x][y - 1] == "a":
                return True
        return False  # no adjacent apples on row or column to coordinates(x, y)

    def move_body_snake(self, direction: str, x: int, y: int) -> tuple:
        """
        Will move the body of the snake accordingly and
        returns a tuple with the new coordinates from which the body continues to move.
        Puts "+"(body part) relative to the direction of x and y.
        :param direction: direction
        :param x: x-coord
        :param y: y-coord
        :return: tuple with new coordinates
        """
        if direction == "left":
            new_x = x
            new_y = y - 1
        elif direction == "right":
            new_x = x
            new_y = y + 1
        elif direction == "up":
            new_x = x - 1
            new_y = y
        else:  # direction == "down"
            new_x = x + 1
            new_y = y
        self.board.data[new_x][new_y] = "+"
        return new_x, new_y

    def move_snake_no_apple(self, direction: str, x: int, y: int):
        if direction == "left":
            new_x = x
            new_y = y - 1
        elif direction == "right":
            new_x = x
            new_y = y + 1
        elif direction == "up":
            new_x = x - 1
            new_y = y
        else:  # direction == "down"
            new_x = x + 1
            new_y = y
        self.board.data[new_x][new_y] = " "

    def move_snake(self, direction: str):
        if direction != "up" and direction != "down" and direction != "right" and direction != "left":
            raise SnakeError("Invalid direction of snake.")
        self.was_apple = False
        new_x = None
        new_y = None
        if direction == "right":
            new_x = self.snake.head_x
            new_y = self.snake.head_y + 1
        if direction == "left":
            new_x = self.snake.head_x
            new_y = self.snake.head_y - 1
        if direction == "up":
            new_x = self.snake.head_x - 1
            new_y = self.snake.head_y
        if direction == "down":
            new_x = self.snake.head_x + 1
            new_y = self.snake.head_y
        if new_x < 0 or new_x == self.board.size or new_y < 0 or new_y == self.board.size:
            raise GameOver("Snake out of board, game over")
        if self.board.data[new_x][new_y] == "+":
            raise GameOver("Snake is tangled, game over.")
        if self.board.data[new_x][new_y] == "a":
            self.was_apple = True
        # if all tests passed, move the head of the snake
        # move on the board
        self.board.data[new_x][new_y] = "*"
        # move snake head in snake object
        self.snake.head_x = new_x
        self.snake.head_y = new_y
        # append move to moves
        self.moves.append(direction)
        moving_x = self.snake.head_x
        moving_y = self.snake.head_y
        for i in range(len(self.moves) - 1, 0, -1):
            moving_x, moving_y = self.move_body_snake(self.__opposite_directions[self.moves[i]], moving_x, moving_y)

        # if an apple was there, the snake increases, if not, the square is just emptied
        if self.was_apple:
            self.move_body_snake(self.__opposite_directions[self.moves[0]], moving_x, moving_y)
            self.place_apples(1)  # add an apple if it was eaten by the snake
        else:
            self.move_snake_no_apple(self.__opposite_directions[self.moves[0]], moving_x, moving_y)

    @property
    def board(self):
        return self.__board

    @board.setter
    def board(self, board: Board):
        self.__board = board

    @property
    def snake(self):
        return self.__snake

    @snake.setter
    def snake(self, snake: Snake):
        self.__snake = snake

    @property
    def moves(self):
        return self.__moves

    @moves.setter
    def moves(self, moves: list):
        self.__moves = moves

    @property
    def was_apple(self):
        return self.__was_apple

    @was_apple.setter
    def was_apple(self, b: bool):
        self.__was_apple = b
