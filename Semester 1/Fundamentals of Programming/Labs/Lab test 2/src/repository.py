from domain import Player

class Repository:
    def __init__(self):
        self.players = []
        self.path = "players.txt"

    def load_file(self):
        with open(self.path, "r") as file:
            lines = file.readlines()
            for line in lines:
                params = line.split(",")
                self.players.append(Player(int(params[0]), params[1], int(params[2])))
        self.players.sort(key=lambda player: -player.strength)

    @property
    def players(self):
        return self.__players

    @players.setter
    def players(self, players: list):
        self.__players = players
