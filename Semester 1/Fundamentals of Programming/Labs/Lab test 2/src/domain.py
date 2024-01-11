class PlayerException(Exception):
    pass

class Player:
    def __init__(self, id: int, name: str, strength: int):
        self.id = id
        self.name = name
        self.strength = strength

    def __eq__(self, other):
        if not isinstance(other, Player):
            raise PlayerException("Not a player")
        return self.id == other.id

    def __str__(self):
        return "ID: " + str(self.id) + "| Name: " + self.name + "| Strength: " + str(self.strength)

    @property
    def id(self):
        return self.__id

    @id.setter
    def id(self, value: int):
        self.__id = value

    @property
    def name(self):
        return self.__name

    @name.setter
    def name(self, string: str):
        self.__name = string

    @property
    def strength(self):
        return self.__strength

    @strength.setter
    def strength(self, value: int):
        self.__strength = value
