class Taxi:
    def __init__(self, id: int, x: int, y: int):
        self.id = id
        self.x = x
        self.y = y
        self.fare = 0

    def __eq__(self, other):
        if not isinstance(other, Taxi):
            raise ValueError("Not a taxi")
        return (self.x == other.x and self.y == other.y) or self.id == other.id

    @property
    def id(self):
        return self.__id

    @id.setter
    def id(self, var: int):
        self.__id = var

    @property
    def x(self):
        return self.__x

    @x.setter
    def x(self, var: int):
        self.__x = var

    @property
    def y(self):
        return self.__y

    @y.setter
    def y(self, var: int):
        self.__y = var

    @property
    def fare(self):
        return self.__fare

    @fare.setter
    def fare(self, var: int):
        self.__fare = var

class City:
    def __init__(self):
        self.data = [[0 for _ in range(100)] for _ in range(100)]

if __name__ == "__main__":
    city = City()
    print(city.data[0][0])

