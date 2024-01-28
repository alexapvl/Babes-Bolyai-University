class City:
    def __init__(self):
        self.year = 1  # year of management
        self.starved = 0  # population which died
        self.population = 100  # people in the city
        self.land = 1000  # acres
        self.harvest = 3  # units/acre
        self.ate_by_rats = 200  # units
        self.grain_stocks = 2800  # units
        self.land_price = 20  # units per acre

    @property
    def year(self):
        return self.__year

    @year.setter
    def year(self, value):
        self.__year = value

    @property
    def starved(self):
        return self.__starved

    @starved.setter
    def starved(self, value):
        self.__starved = value

    @property
    def population(self):
        return self.__population

    @population.setter
    def population(self, value):
        self.__population = value

    @property
    def land(self):
        return self.__land

    @land.setter
    def land(self, value):
        self.__land = value

    @property
    def harvest(self):
        return self.__harvest

    @harvest.setter
    def harvest(self, value):
        self.__harvest = value

    @property
    def ate_by_rats(self):
        return self.__ate_by_rats

    @ate_by_rats.setter
    def ate_by_rats(self, value):
        self.__ate_by_rats = value

    @property
    def grain_stocks(self):
        return self.__grain_stocks

    @grain_stocks.setter
    def grain_stocks(self, value):
        self.__grain_stocks = value

    @property
    def land_price(self):
        return self.__land_price

    @land_price.setter
    def land_price(self, value):
        self.__land_price = value
