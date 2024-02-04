from src.domain.snake import Snake
from src.domain.board import Board
from src.service.service import Service
from src.ui.ui import UI

if __name__ == '__main__':
    size, apples = Service.get_settings()
    board = Board(size, apples)
    snake = Snake()
    service = Service(board, snake)
    ui = UI(service)
    ui.start()
