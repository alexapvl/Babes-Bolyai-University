from src.service.service import GameService
from colorama import Fore, Style


class GameUI:
    def __init__(self, game_service: GameService):
        self.game = game_service
        self.start_with_ai = False
        self.in_turn = 1

    def start(self):
        continue_game = True
        while self.game.difficulty is None:
            difficulty = input(Fore.YELLOW + "Choose the difficulty of the game:\n"
                               + Fore.GREEN + "e. Easy\n"
                               + Fore.RED + "h. Hard\n" + Style.RESET_ALL)
            if difficulty == 'e':
                self.game.difficulty = "easy"
            elif difficulty == 'h':
                self.game.difficulty = "hard"
            else:
                print(Fore.RED + "Invalid option" + Style.RESET_ALL)
        while continue_game:
            try:
                x = int(input(Fore.YELLOW + "\nGive the dimension of the board: \nheight = " + Style.RESET_ALL))
                if x < 1:
                    raise ValueError
                y = int(input(Fore.YELLOW + "width = " + Style.RESET_ALL))
                if y < 1:
                    raise ValueError

                set_player = input(Fore.YELLOW + "\nDo you want to be the one that starts the game?\n"
                                   + Fore.GREEN + "y. Yes\n"
                                   + Fore.RED + "n. No\n" + Style.RESET_ALL)
                # Setting the size of the board
                self.game.board.row = x
                self.game.board.column = y
                self.game.create_board()

                if set_player == 'y':
                    self.in_turn = 1  # 1 will be the player
                    self.start_with_ai = False
                elif set_player == 'n':
                    self.in_turn = 2  # 2 will be the computer
                    self.start_with_ai = True
                while self.game.game_over():
                    if self.in_turn == 1:
                        try:
                            print(str(self.game.board))
                            x = input("x = ")
                            y = input("y = ")
                            self.game.make_move_player(int(x), int(y))
                            self.in_turn = 2
                        except Exception as error:
                            print(Fore.RED + str(error) + Style.RESET_ALL)
                    elif self.in_turn == 2:
                        self.game.make_move_ai(self.start_with_ai, x, y)
                        self.in_turn = 1
                print(str(self.game.board))
                if self.in_turn == 2:
                    print(Fore.YELLOW + "Player has won" + Style.RESET_ALL)
                else:
                    print(Fore.YELLOW + "Computer has won" + Style.RESET_ALL)

                cont = input(Fore.YELLOW + "\n Do you wish to play again?\n"
                             + Fore.GREEN + "y. Yes\n"
                             + Fore.RED + "n. No\n" + Style.RESET_ALL)
                if cont == 'y':
                    self.game.destroy_board()
                elif cont == 'n':
                    continue_game = False
                    print(Fore.MAGENTA + "Bye!" + Style.RESET_ALL)
                else:
                    print(Fore.RED + "Invalid option" + Style.RESET_ALL)
            except ValueError:
                print(
                    Fore.RED + "Invalid coordinates \nHeight and width must be greater or equal than 1" + Style.RESET_ALL)

    @property
    def game(self):
        return self.__game

    @game.setter
    def game(self, service: GameService):
        self.__game = service

    @property
    def start_with_ai(self):
        return self.__start_with_ai

    @start_with_ai.setter
    def start_with_ai(self, value: bool):
        self.__start_with_ai = value

    @property
    def in_turn(self):
        return self.__in_turn

    @in_turn.setter
    def in_turn(self, value: int):
        self.__in_turn = value
