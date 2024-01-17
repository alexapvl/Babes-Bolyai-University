from src.domain.point import Point

class Board:
    def __init__(self, row=0, column=0):
        self.row = row
        self.column = column
        self.board = []

    def create_board(self):
        """
        This function creates the board with the corresponding dimension.
        The board will be a list of lists, each list will represent a row, and each element of these lists will
        represent a column.
        The board will have self.row lists and each list will have self.column elements.
        """
        for x in range(self.row):
            array = []
            for y in range(self.column):
                array.append(0)
            self.board.append(array)

    def destroy_board(self):
        """
        Function that sets the board to the default size (check '__init__' function)
        """
        self.board = []
        self.row = 0
        self.column = 0

    def clear_board(self):
        """
        Function that clears the board
        """
        for x in range(self.row):
            for y in range(self.column):
                self.board[x][y] = 0

    def move(self, move: Point, player: str):
        """
        Function that makes a move
        """
        self.board[move.x][move.y] = player
        self.border_move(move)

    def border_move(self, point: Point):
        """
        Function that borders all the existing neighbours of a point, if they exist, and they are not bordered
        already
        """
        x = point.x
        y = point.y
        if x - 1 >= 0 and y - 1 >= 0 and self.board[x - 1][y - 1] == 0:
            self.board[x - 1][y - 1] = " - "
        if x - 1 >= 0 and self.board[x - 1][y] == 0:
            self.board[x - 1][y] = " - "
        if x - 1 >= 0 and y + 1 < self.column and self.board[x - 1][y + 1] == 0:
            self.board[x - 1][y + 1] = " - "
        if y - 1 >= 0 and self.board[x][y - 1] == 0:
            self.board[x][y - 1] = " - "
        if y + 1 < self.column and self.board[x][y + 1] == 0:
            self.board[x][y + 1] = " - "
        if x + 1 < self.row and y + 1 < self.column and self.board[x + 1][y + 1] == 0:
            self.board[x + 1][y + 1] = " - "
        if x + 1 < self.row and self.board[x + 1][y] == 0:
            self.board[x + 1][y] = " - "
        if x + 1 < self.row and y - 1 >= 0 and self.board[x + 1][y - 1] == 0:
            self.board[x + 1][y - 1] = " - "

    def unborder_move(self, point: Point):
        """
        Function that un-borders all the existing neighbours of a point, if they exist, and they are not bordered
        already
        """
        x = point.x
        y = point.y
        if x - 1 >= 0 and y - 1 >= 0 and self.board[x - 1][y - 1] == " - ":
            self.board[x - 1][y - 1] = 0
        if x - 1 >= 0 and self.board[x - 1][y] == " - ":
            self.board[x - 1][y] = 0
        if x - 1 >= 0 and y + 1 < self.column and self.board[x - 1][y + 1] == " - ":
            self.board[x - 1][y + 1] = 0
        if y - 1 >= 0 and self.board[x][y - 1] == " - ":
            self.board[x][y - 1] = 0
        if y + 1 < self.column and self.board[x][y + 1] == " - ":
            self.board[x][y + 1] = 0
        if x + 1 < self.row and y + 1 < self.column and self.board[x + 1][y + 1] == " - ":
            self.board[x + 1][y + 1] = 0
        if x + 1 < self.row and self.board[x + 1][y] == " - ":
            self.board[x + 1][y] = 0
        if x + 1 < self.row and y - 1 >= 0 and self.board[x + 1][y - 1] == " - ":
            self.board[x + 1][y - 1] = 0

    def available_moves(self) -> list:
        """
        Function that returns the available moves
        """
        moves = []
        for x in range(self.row):
            for y in range(self.column):
                if self.board[x][y] == 0:
                    moves.append(Point(x, y))
        return moves

    def count_x_and_o(self) -> tuple:
        """
        Function that counts the number of X's on the board
        """
        count_x = 0
        count_o = 0
        for x in range(self.row):
            for y in range(self.column):
                if self.board[x][y] == 'X':
                    count_x += 1
                if self.board[x][y] == 'O':
                    count_o += 1
        return count_x, count_o

    def fix(self):
        """
        Function that fixes the board
        Used when undoing a move, to prevent the board from having deleted common neighbour fillers
        """
        for x in range(self.row):
            for y in range(self.column):
                if self.board[x][y] == 'X' or self.board[x][y] == 'O':
                    self.border_move(Point(x, y))

    def undo(self, move: Point):
        """
        Function that undoes a move
        """
        self.board[move.x][move.y] = 0
        self.unborder_move(move)
        self.fix()

    def __len__(self):
        """
        Overriding the len function
        """
        return len(self.board)

    def __str__(self):
        """
        Overriding the str function
        """
        string = "\n   "
        for x in range(self.column):
            string += str(x) + '   '
        for x in range(self.row):
            string += "\n "
            string += "-" * (4 * self.column + 1)
            string += "\n"
            string += str(x) + '|'
            for y in range(self.column):
                if self.board[x][y] == 'O':  # the human player is represented by 'O'
                    string += ' ' + self.board[x][y] + ' ' + "|"
                elif self.board[x][y] == 'X':  # the computer is represented by 'X'
                    string += ' ' + self.board[x][y] + ' ' + "|"
                elif self.board[x][y] == " - ":  # nonempty neighbours -> fillers
                    string += self.board[x][y] + "|"
                else:
                    string += ' ' + ' ' + ' ' + "|"  # when element is 0 and square was not occupied
        string += "\n "
        string += "-" * (4 * self.column + 1) + "\n"
        return string

    @property
    def row(self):
        return self.__row

    @row.setter
    def row(self, value: int):
        self.__row = value

    @property
    def column(self):
        return self.__column

    @column.setter
    def column(self, value: int):
        self.__column = value

    @property
    def board(self):
        return self.__board

    @board.setter
    def board(self, board: list):
        self.__board = board

class ValidatePoint:
    @staticmethod
    def valid_point(x, y, board: Board):
        """
        Function that validates the coordinates of a point given
        """
        try:
            x = int(x)
            y = int(y)
        except ValueError:
            raise Exception("Please give integers")
        if y < 0 or x < 0 or y >= board.column or x >= board.row:
            raise Exception("Point out of border")
        if board.board[x][y] == " - " or board.board[x][y] == 'O' or board.board[x][y] == 'X':
            raise Exception("Square already taken")
