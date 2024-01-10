from domain import City, Taxi

class Repository:
    def __init__(self):
        self.taxis = []
        self.city = City()

    def add_taxi(self, taxi: Taxi):
        self.taxis.append(taxi)
        self.city.data[taxi.x][taxi.y] = taxi

    def ride(self, x1: int, y1: int, x2: int, y2: int):
        index = self.closest_taxi_to_given_point(x1, y1)
        self.city.data[self.taxis[index].x][self.taxis[index].y] = 0
        self.city.data[x2][y2] = self.taxis[index]
        self.taxis[index].fare += self.hamming_distance(x1, y1, x2, y2)
        self.taxis[index].x = x2
        self.taxis[index].y = y2

    def closest_taxi_to_given_point(self, x: int, y: int) -> int:
        closest_distance = 1000
        taxi_index = None
        for index in range(len(self.taxis)):
            if self.hamming_distance(self.taxis[index].x, self.taxis[index].y, x, y) < closest_distance:
                closest_distance = self.hamming_distance(self.taxis[index].x, self.taxis[index].y, x, y)
                taxi_index = index

        return taxi_index

    @staticmethod
    def hamming_distance(x1: int, y1: int, x2: int, y2: int) -> int:
        return abs(x2 - x1) + abs(y2 - y1)
