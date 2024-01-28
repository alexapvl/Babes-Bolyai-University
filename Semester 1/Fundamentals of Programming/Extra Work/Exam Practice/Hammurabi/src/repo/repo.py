from src.domain.city import City

class RepoError(ValueError):
    pass

class Repository:
    def __init__(self, city: City):
        self.city = city

    # here I will put the functions that impact the city based on my decisions
    # acres to sell
    def buy_sell_acres(self, value: int):
        """
        Number of acres to buy or sell.
        Also, we make checks to see if we can perform the transaction of land.
        :param value: number of acres
        """
        if value > 0 and self.city.land_price * value > self.city.grain_stocks:
            # check if we have enough units to buy the amount of land
            raise RepoError("Not enough units to buy this amount of land.")
        if value < 0 and (-1) * value > self.city.land:
            # check if we can sell the amount of land
            raise RepoError("Cannot sell more land than you own.")
        self.city.grain_stocks += self.city.land_price * (-1) * value
        self.city.land += value

    # feed population
    def feed_population(self, value) -> int:
        """
        Feed population.
        Also, we make some checks to see if we can provide the units to be fed.
        If not all population is fed, then people starve.
        :param value: number of units of grain to be fed
        :return: the number of people that starved
        """
        if value < 0:
            raise RepoError("Invalid input, you cannot feed with negative grains.")
        if value > self.city.grain_stocks:
            # check if we have the desired amount of units to feed the population
            raise RepoError("Not enough units to feed.")
        self.city.grain_stocks -= value
        people_not_fed = self.city.population - (value // 20)
        if people_not_fed > 0:
            self.city.population -= people_not_fed
            return people_not_fed
        return 0

    # plant
    def acres_to_plant(self, value):
        """
        The acres to be planted will be planted in the next year, so we just check if we can plant those acres or not
        :param value: number of acres
        """
        if value < 0:
            raise RepoError("Invalid input, you cannot plant negative acres.")
        if value > self.city.land:
            # check if we can plant the amount of acres that we want
            raise RepoError("You cannot plant more acres than you have.")
        if value > self.city.grain_stocks:
            # check if we have enough grains to plant
            raise RepoError("Not enough grains to be planted.")
        if value > self.city.population * 10:
            raise RepoError("Not enough people to plant all the grains.")
        self.city.grain_stocks -= value

    @property
    def city(self):
        return self.__city

    @city.setter
    def city(self, city: City):
        self.__city = city
