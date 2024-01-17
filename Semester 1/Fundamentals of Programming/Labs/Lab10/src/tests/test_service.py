from src.domain.board import Board
from src.service.service import GameService
from unittest import TestCase

class TestService(TestCase):
    # test if a move is made
    def test_make_move_player(self):
        board = Board()
        board.row = 5
        board.column = 5
        board.create_board()
        game = GameService(board)
        game.make_move_player(1, 1)
        self.assertEqual(game.board.board[1][1], 'O')

    # test mirror move
    def test_mirror_move(self):
        board = Board()
        board.row = 5
        board.column = 5
        board.create_board()
        game = GameService(board)
        game.make_move_player(1, 1)
        mirror = game.mirror(1, 1)
        self.assertEqual(mirror.x, 3)
        self.assertEqual(mirror.y, 3)

    # test if the next turn is correct
    def test_next_turn(self):
        board = Board()
        board.row = 5
        board.column = 5
        board.create_board()
        game = GameService(board)
        game.make_move_player(1, 1)
        next_move = game.next_turn(game.board)
        self.assertEqual(next_move, 'MAX')
