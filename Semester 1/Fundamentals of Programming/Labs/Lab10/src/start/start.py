from src.domain.board import Board
from src.service.service import GameService
from src.ui.ui import GameUI
from src.gui.gui import GameGUI
from src.settings.settings import GetSettings

class StartGame:
    def __init__(self):
        self.game = GameService(Board())

    def start(self):
        game_mode = GetSettings.get_settings()
        if game_mode == "UI":
            ui = GameUI(self.game)
            ui.start()
        elif game_mode == "GUI":
            board = Board()
            game_service = GameService(board)
            gui = GameGUI(game_service)
        else:
            raise ValueError("Invalid game mode")

    @property
    def game(self):
        return self.__game

    @game.setter
    def game(self, service: GameService):
        self.__game = service

if __name__ == "__main__":
    game = StartGame()
    game.start()
