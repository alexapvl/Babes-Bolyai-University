from unittest import TestCase
from src.domain.board import Board
from src.domain.point import Point

class TestBoard(TestCase):
    # create a board and test if it has the right size
    def test_create_board(self):
        board = Board(4, 4)
        self.assertEqual(board.row, 4)
        self.assertEqual(board.column, 4)

    # test if the board is destroyed
    def test_destroy_board(self):
        board = Board(4, 4)
        board.destroy_board()
        self.assertEqual(board.row, 0)
        self.assertEqual(board.column, 0)

    # test if the board is cleared
    def test_clear_board(self):
        board = Board()
        board.row = 4
        board.column = 4
        board.create_board()
        board.clear_board()
        for i in range(board.row):
            for j in range(board.column):
                self.assertEqual(board.board[i][j], 0)

    # test if a move is made
    def test_move(self):
        board = Board()
        board.row = 4
        board.column = 4
        board.create_board()
        point = Point(1, 1)
        board.move(point, 'X')
        self.assertEqual(board.board[point.x][point.y], 'X')

    # test if the neighbours of a point are bordered
    def test_border_move(self):
        board = Board()
        board.row = 5
        board.column = 5
        board.create_board()
        point = Point(2, 2)
        board.move(point, 'X')
        self.assertEqual(board.board[point.x - 1][point.y - 1], ' - ')
        self.assertEqual(board.board[point.x - 1][point.y], ' - ')
        self.assertEqual(board.board[point.x - 1][point.y + 1], ' - ')
        self.assertEqual(board.board[point.x][point.y - 1], ' - ')
        self.assertEqual(board.board[point.x][point.y + 1], ' - ')
        self.assertEqual(board.board[point.x + 1][point.y - 1], ' - ')
        self.assertEqual(board.board[point.x + 1][point.y], ' - ')
        self.assertEqual(board.board[point.x + 1][point.y + 1], ' - ')
