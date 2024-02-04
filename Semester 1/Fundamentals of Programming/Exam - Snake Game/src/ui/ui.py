from src.domain.snake import SnakeError
from src.service.service import Service, GameOver

class UIException(Exception):
    pass

class UI:
    def __init__(self, service: Service):
        self.service = service

    def print_board(self):
        print(self.service.board)

    def start(self):
        while True:
            try:
                self.print_board()
                command = input(">>> ")
                params = command.split()
                if len(params) == 1:
                    if params[0] == "right" or params[0] == "left" or params[0] == "up" or params[0] == "down":
                        if params[0] == self.service.moves[-1]:
                            # make no move if the snake is oriented in the same direction as command
                            # only move snake in the direction it is facing when performing move
                            continue
                        else:
                            self.service.move_snake(params[0])
                    elif params[0] == "move":
                        # move in the same direction that the snake is facing
                        self.service.move_snake(self.service.moves[-1])
                elif len(params) == 2:
                    if params[0] == "move":
                        if params[1].isdigit():
                            number_moves = int(params[1])
                            for i in range(number_moves):
                                # move in the same direction that the snake is facing
                                self.service.move_snake(self.service.moves[-1])
                        else:
                            raise UIException("Invalid command")
                    else:
                        raise UIException("Invalid command.")
            except SnakeError as se:
                print(se)
            except UIException as uie:
                print(uie)
            except GameOver as go:
                print(go)
                break  # exits the while loop and exits the game

    @property
    def service(self):
        return self.__service

    @service.setter
    def service(self, service: Service):
        self.__service = service
