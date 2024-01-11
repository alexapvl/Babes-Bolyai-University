from domain import Taxi
from repository import Repository
from random import randint

class Service:
    def __init__(self, repo: Repository):
        self.repo = repo

    def generate_n_random_taxis(self, n: int):
        for id in range(n):
            taxi = Taxi(id, randint(0, 99), randint(0, 99))
            while not self.hamming_distance_ok(taxi):
                taxi = Taxi(id, randint(0, 99), randint(0, 99))
            self.add_taxi(taxi)

    def generate_valid_coordinates(self) -> tuple:
        x1 = randint(0, 99)
        y1 = randint(0, 99)
        x2 = randint(0, 99)
        y2 = randint(0, 99)
        while self.repo.city.data[x2][y2] != 0 or not self.exists_ride_at_least_10_long(x2, y2):
            x2 = randint(0, 99)
            y2 = randint(0, 99)
        return x1, y1, x2, y2

    def add_taxi(self, taxi: Taxi):
        self.repo.add_taxi(taxi)

    def ride(self, x1: int, y1: int, x2: int, y2: int):
        self.repo.ride(x1, y1, x2, y2)

    def hamming_distance_ok(self, taxi: Taxi) -> bool:
        for t in self.repo.taxis:
            if self.repo.hamming_distance(t.x, t.y, taxi.x, taxi.y) <= 5:
                return False
        return True

    def exists_ride_at_least_10_long(self, x: int, y: int) -> bool:
        for taxi in self.repo.taxis:
            if self.repo.hamming_distance(x, y, taxi.x, taxi.y) > 10:
                return True
        return False
