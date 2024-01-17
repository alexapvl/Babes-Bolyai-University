class Point:
    def __init__(self, x, y):
        self.x = int(x)
        self.y = int(y)

    def __str__(self):
        return str(self.x) + " " + str(self.y)

    @property
    def x(self):
        return self.__x

    @x.setter
    def x(self, value):
        self.__x = int(value)

    @property
    def y(self):
        return self.__y

    @y.setter
    def y(self, value):
        self.__y = int(value)
