from src.domain.point import Point
from src.domain.board import ValidatePoint, Board
from unittest import TestCase

class TestPoint(TestCase):
    # create a point and test if it has the right coordinates
    def test_create_point(self):
        point = Point(1, 1)
        self.assertEqual(point.x, 1)
        self.assertEqual(point.y, 1)

    # validate a point
    def test_validate_point(self):
        board = Board()
        board.row = 5
        board.column = 5
        board.create_board()
        point = Point(-1, -1)
        self.assertRaises(Exception, ValidatePoint.valid_point, point.x, point.y, board)
