from domain import Player, PlayerException
from repository import  Repository
from services import Service


class UI:
    def __init__(self, service: Service):
        self.service = service
        self.remaining_players = self.service.repo.players.copy()

    def start(self):
        while True:
            self.menu()
            try:
                option = input(">>> ")
                if option == "1":
                    self.display()
                if option == "2":
                    print(f"Last {len(self.remaining_players)}")
                    # start tournament
                    if len(self.remaining_players) % 2 == 0:
                        print("Starting tournament...")
                        pairings = self.service.pair_players(self.remaining_players)
                        for i in range(len(pairings)):
                            while True:
                                print("Current round")
                                player1, player2 = pairings[i]
                                print(player1)
                                print(player2)
                                who_wins = input("Who wins? 1/2 -> ")
                                if who_wins == "1":
                                    player1.strength += 1
                                    self.service.update_player(player1)
                                    self.remaining_players.remove(player2)
                                    break
                                elif who_wins == "2":
                                    player2.strength += 1
                                    self.service.update_player(player2)
                                    self.remaining_players.remove(player1)
                                    break
                                else:
                                    print("Invalid option")
                    else:
                        # while len(self.remaining_players) % 2 != 0:
                        ...
                    if len(self.remaining_players) == 2:
                        print("Final round...")
                        while True:
                            print("Current round")
                            player1 = self.remaining_players[0]
                            player2 = self.remaining_players[1]
                            print(player1)
                            print(player2)
                            who_wins = input("Who wins? 1/2 -> ")
                            if who_wins == "1":
                                player1.strength += 1
                                self.service.update_player(player1)
                                self.remaining_players.remove(player2)
                                print(f"{player1.name} WINS")
                                break
                            elif who_wins == "2":
                                player2.strength += 1
                                self.service.update_player(player2)
                                self.remaining_players.remove(player1)
                                print(f"{player2.name} WINS")
                                break
                            else:
                                print("Invalid option")
                    if len(self.remaining_players) == 1:
                        print(f"{self.remaining_players[0]} already won")

            except ValueError as ve:
                print(ve)
            except PlayerException as pe:
                print(pe)

    def display(self):
        for player in self.service.repo.players:
            print(player)

    @staticmethod
    def menu():
        print()
        print("1. Display players")
        print("2. Play next round")
        print("0. Exit")
        print()

if __name__ == "__main__":
    repo = Repository()
    repo.load_file()
    services = Service(repo)
    ui = UI(services)
    ui.start()
