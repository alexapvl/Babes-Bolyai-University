from src.domain.city import City
from src.repo.repo import RepoError, Repository
from src.service.city_management import CityManagement

class UI:
    def __init__(self, management: CityManagement):
        self.service = management
        self.starved_people = 0
        self.new_people = 0
        self.game_over = False

    def yearly_report(self):
        print(f"\nIn year {self.service.repo.city.year}, {self.starved_people} people have starved.")
        print(f"{self.new_people} people came to the city.")
        print(f"City population is {self.service.repo.city.population}.")
        print(f"City owns {self.service.repo.city.land} acres of land.")
        print(f"Harvest was {self.service.repo.city.harvest} units per acre.")
        print(f"Rats ate {self.service.repo.city.ate_by_rats} units.")
        print(f"Land price is {self.service.repo.city.land_price} units per acre.")
        print(f"\nGrain stocks are {self.service.repo.city.grain_stocks} units.\n")

    def final_report(self):
        print(f"\nIn year {self.service.repo.city.year}, {self.starved_people} people have starved.")
        print(f"City population is {self.service.repo.city.population}.")
        print(f"City owns {self.service.repo.city.land} acres of land.")
        print(f"Harvest was {self.service.repo.city.harvest} units per acre.")
        print(f"Rats ate {self.service.repo.city.ate_by_rats} units.")
        print(f"Land price is {self.service.repo.city.land_price} units per acre.\n")

        if self.service.repo.city.population > 100 and self.service.repo.city.land > 1000:
            print("GAME OVER. You did well.")
        else:
            print("GAME OVER. You did not do well.")

    @staticmethod
    def forced_game_over():
        print("\nMore than half of your people died last year, you lost.")

    def start(self):
        while self.service.repo.city.year < 5:
            if self.starved_people > self.service.repo.city.population:
                UI.forced_game_over()
                self.game_over = True
                break
            if self.starved_people == 0 and self.service.repo.city.year != 1:
                self.new_people = self.service.new_people_to_town()
            self.yearly_report()  # print report
            while True:
                try:
                    acres_to_buy = int(input("Acres to buy/sell (+/-) -> "))
                    # update acres, stock
                    self.service.repo.buy_sell_acres(acres_to_buy)
                    print(self.service.repo.city.grain_stocks)
                    break
                except RepoError as re:
                    print(re)
            while True:
                try:
                    units_to_feed_population = int(input("Units to feed population -> "))
                    self.starved_people = self.service.repo.feed_population(units_to_feed_population)
                    print(self.service.repo.city.grain_stocks)
                    break
                except RepoError as re:
                    print(re)
            while True:
                try:
                    acres_to_plant = int(input("Acres to plant -> "))
                    self.service.repo.acres_to_plant(acres_to_plant)
                    print(self.service.repo.city.grain_stocks)
                    break
                except RepoError as re:
                    print(re)
            # update the randomised things
            self.service.update_land_prices()
            self.service.harvest_per_unit()
            self.service.repo.city.grain_stocks += self.service.repo.city.harvest * acres_to_plant  # update the stock after plantation and harvesting rate changes
            self.service.rat_infestation()
            self.service.repo.city.year += 1  # increase the year by one

        # when the game is over i.e. year = 5
        if not self.game_over:
            self.final_report()

    @property
    def service(self):
        return self.__service

    @service.setter
    def service(self, management: CityManagement):
        self.__service = management

    @property
    def starved_people(self):
        return self.__starved_people

    @starved_people.setter
    def starved_people(self, value):
        self.__starved_people = value

if __name__ == '__main__':
    city = City()
    repo = Repository(city)
    management = CityManagement(repo)
    ui = UI(management)
    ui.start()
