from src.domain.board import ValidatePoint
from src.domain.point import Point
from src.domain.board import Board
from random import randint

class GameService:
    def __init__(self, board: Board):
        self.board = board
        self.valid = ValidatePoint()
        self.start_ai = False  # Variable which tells us if AI is the first to make a move on an odd board (if 'X' is the first to make a move or not)
        self.difficulty = None

    def create_board(self):
        """
        Creates the board and sets the start_odd variable to its default value
        """
        self.board.create_board()
        self.start_ai = False

    def destroy_board(self):
        """
        Function that destroys the board
        """
        self.board.destroy_board()

    def make_move_player(self, x: int, y: int):
        """
        Player's move has value 1
        Function that validates the move the player wants to make.
        Raises exception if the move is invalid or if the square is unavailable.
        Otherwise, it records the move.
        """
        self.valid.valid_point(x, y, self.board)
        point = Point(x, y)
        self.board.move(point, 'O')

    def make_move_ai(self, computer: bool, x: int, y: int):
        """
        Function that makes the AI's move
        """
        moves = self.board.available_moves()
        if self.difficulty == "easy":
            self.random_move(moves)
        else:
            row = self.board.row
            column = self.board.column
            next_move = self.decide_move(computer, row, column, moves)
            if next_move == 1:  # Strategy for odd boards
                self.ai_odd(row, column, moves, x, y)
            elif next_move == 2:  # Minimax move
                value, move = self.mini_max(self.board)
                self.board.move(move, 'X')
            elif next_move == 3:  # Random move
                self.random_move(moves)

    def mirror(self, x, y):
        """
        Function that returns a Point that represents the mirrored move of the player
        (strategy used by AI when it is the one to make the first move on an odd by odd board)
        """
        row = self.board.row
        col = self.board.column

        if self.board.board[row - x - 1][col - y - 1] == 0:
            return Point(row - x - 1, col - y - 1)

        if self.board.board[x][col - y - 1] == 0:
            return Point(x, col - y - 1)

        if self.board.board[row - x - 1][y] == 0:
            return Point(row - x - 1, y)

    def decide_move(self, computer: bool, row: int, column: int, moves: list):
        """
        Function that decides how will the AI make its next move based on some criteria
        OBS: If the AI starts we check if the board has odd coordinates
        """
        if computer is True and len(moves) == row * column:
            if self.check_odd(row, column):
                return 1
            else:
                # If the board is not odd x odd, the first move of AI will be random
                return 3

        # If the board is odd and AI started we continue with 1
        if self.start_ai is True and len(moves) != row * column:
            if self.check_odd(row, column):
                return 1
            else:
                # If the board is not odd x odd and the AI started, the AI will make a minmax move
                return 2

        # If the above criteria wasn't met, the AI will just make its move based on the minimax algorithm
        return 2

    def ai_odd(self, row: int, column: int, moves: list, x: int, y: int):
        """
        Function that follows the next strategy:
        If the board is odd and AI makes the first move, then the first move will be in the center of the board.
        In order to win, next the AI will only mirror the player's moves.
        """
        # If it is the first move, AI takes the central square
        if len(moves) == row * column:
            point = Point(row // 2, column // 2)
            self.board.move(point, 'X')
            self.start_ai = True

        # If the board is odd and AI started, it will mirror the player's move in order to win
        if len(moves) != row * column:
            move = self.mirror(int(x), int(y))
            self.board.move(move, 'X')

    def random_move(self, moves: list):
        """
        Function that makes AI move randomly
        """
        move = randint(0, len(moves) - 1)
        self.board.move(moves[move], 'X')

    def mini_max(self, game_state: Board, alpha=float('-inf'), beta=float('inf')) -> tuple:
        """
        Function that implements the minimax algorithm using alpha beta pruning
        This is a recursive function which calls itself until it reaches a terminal state of the board
        Then it returns the value of the terminal state and None as best move because there is no possible move to be made
        :param game_state: The current state of the board
        :param alpha: variable used in alpha beta pruning
        :param beta: variable used in alpha beta pruning
        :return: returns the value of the best move and the best move itself
        """
        if len(game_state.available_moves()) == 0:
            return self.value_of_terminal_state_of_game(game_state), None

        if self.next_turn(game_state) == "MAX":
            best_value = float('-inf')
            best_move = None
            for move in game_state.available_moves():
                game_state.move(move, 'X')
                value, _ = self.mini_max(game_state, alpha, beta)
                # undo move
                game_state.undo(move)
                if value > best_value:
                    best_value = value
                    best_move = move
                alpha = max(alpha, best_value)
                if alpha >= beta:
                    break  # Beta cut-off
            return best_value, best_move

        if self.next_turn(game_state) == "MIN":
            best_value = float('inf')
            best_move = None
            for move in game_state.available_moves():
                game_state.move(move, 'O')
                value, _ = self.mini_max(game_state, alpha, beta)
                # undo move
                game_state.undo(move)
                if value < best_value:
                    best_value = value
                    best_move = move
                beta = min(beta, best_value)
                if alpha >= beta:
                    break  # Alpha cut-off
            return best_value, best_move

    def value_of_terminal_state_of_game(self, board: Board) -> int:
        """
        Function that returns the value of the current terminal state of the game
        In our case, the human is the minimizer and the AI is the maximizer
        If the human wins, the value of the terminal state is -1
        If the AI wins, the value of the terminal state is 1
        OBS: There are no draws in this game, either one wins or the other
        :return: Integer representing the value of the terminal state of the game
        """
        count_x, count_o = board.count_x_and_o()
        if not self.start_ai:
            if count_o > count_x:
                return -1  # human wins
            elif count_o == count_x:
                return 1  # AI wins
        else:
            if count_x > count_o:
                return 1  # AI wins
            elif count_o == count_x:
                return -1  # human wins

    def next_turn(self, board: Board) -> str:
        """
        Function that returns the next turn
        :return: String indicating the next turn
        """
        count_x, count_o = board.count_x_and_o()
        if not self.start_ai:
            if count_o > count_x:
                return "MAX"
            elif count_o == count_x:
                return "MIN"
        else:
            if count_x > count_o:
                return "MIN"
            elif count_o == count_x:
                return "MAX"

    def game_over(self) -> bool:
        """
        Function that returns True if there are still available moves to be made and False otherwise
        """
        if self.board.available_moves():
            return True
        return False

    @staticmethod
    def check_odd(x, y):
        """
        Function that returns true if the board has an odd size and false otherwise
        """
        return x % 2 and y % 2

    @property
    def board(self):
        return self.__board

    @board.setter
    def board(self, value):
        self.__board = value

    @property
    def valid(self):
        return self.__valid

    @valid.setter
    def valid(self, value):
        self.__valid = value

    @property
    def start_ai(self):
        return self.__start_ai

    @start_ai.setter
    def start_ai(self, value):
        self.__start_ai = value
