from repository import Repository
from service import Service

class UI:
    def __init__(self, service: Service):
        self.service = service

    def start(self):
        n = int(input("How many taxis do you want to generate? "))
        self.service.generate_n_random_taxis(n)
        while True:
            self.menu()
            try:
                command = input(">>> ")
                if command == "0":
                    break
                elif command == "1":
                    while True:
                        try:
                            print("Start:")
                            x1 = int(input("x = "))
                            if x1 < 0 or x1 > 99:
                                raise ValueError("Invalid x coord")
                            y1 = int(input("y = "))
                            if y1 < 0 or y1 > 99:
                                raise ValueError("Invalid y coord")
                            break
                        except ValueError as ve:
                            print(ve)
                    while True:
                        try:
                            print("End:")
                            x2 = int(input("x = "))
                            if x2 < 0 or x2 > 99:
                                raise ValueError("Invalid x coord")
                            y2 = int(input("y = "))
                            if y2 < 0 or y2 > 99:
                                raise ValueError("Invalid y coord")
                            if self.service.repo.city.data[x2][y2] != 0:
                                raise ValueError("Taxi on that place already")
                            break
                        except ValueError as ve:
                            print(ve)
                    self.service.ride(x1, y1, x2, y2)
                elif command == "2":
                    x1, y1, x2, y2 = self.service.generate_valid_coordinates()
                    self.service.ride(x1, y1, x2, y2)
                else:
                    raise ValueError("Invalid command")

            except ValueError as ve:
                print(ve)

    @staticmethod
    def menu():
        print()
        print("1. Add a ride")
        print("2. Simulate a ride")
        print("0. Exit")
        print()

if __name__ == "__main__":
    repo = Repository()
    service = Service(repo)
    ui = UI(service)
    ui.start()


