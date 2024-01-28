from src.repo.repo import Repository
from random import randint

class CityManagement:
    def __init__(self, repo: Repository):
        self.repo = repo

    # here I will put the functions that do all the automatic work in the course of the year
    def update_land_prices(self):
        # each year the land price varies from 15 to 25 units per acre at random
        self.repo.city.land_price = randint(15, 25)

    def harvest_per_unit(self):
        # each year the harvest varies from 1 to 6 grains/units per acre
        self.repo.city.harvest = randint(1, 6)

    def rat_infestation(self):
        """
        Tells us if a rat infestation occurred and if it did, what amount of grains were eaten.
        Also, we update the stock of grains.
        """
        infestation = randint(1, 5)
        # 20% chance of it being one value since we have 5 of them
        if infestation == 1:
            # if the infestation occurred, then we take a random value and return it
            # the value is between 0% and 10% of the total grain stock
            self.repo.city.ate_by_rats = randint(0, self.repo.city.grain_stocks // 10)
            self.repo.city.grain_stocks -= self.repo.city.ate_by_rats
        else:
            self.repo.city.ate_by_rats = 0  # no infestation

    def new_people_to_town(self) -> int:
        new_people = randint(1, 10)
        self.repo.city.population += new_people
        return new_people

    @property
    def repo(self):
        return self.__repo

    @repo.setter
    def repo(self, repo: Repository):
        self.__repo = repo
