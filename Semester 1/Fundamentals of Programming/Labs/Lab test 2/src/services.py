from domain import Player
from repository import Repository
from random import randint, shuffle

class Service:
    def __init__(self, repo: Repository):
        self.repo = repo

    def update_player(self, p: Player):
        for i in range(len(self.repo.players)):
            if self.repo.players[i] == p:
                self.repo.players[i].strength = p.strength

    @staticmethod
    def pair_players(player_list: list) -> list:
        """
        Pairs the players randomly
        :param player_list: The list of players
        :return: A list of tuples which has the pairs in it
        """
        result = []
        indexes = [i for i in range(len(player_list))]
        shuffle(indexes)
        for i in range(0, len(indexes), 2):
            result.append((player_list[i], player_list[i+1]))
        return result
