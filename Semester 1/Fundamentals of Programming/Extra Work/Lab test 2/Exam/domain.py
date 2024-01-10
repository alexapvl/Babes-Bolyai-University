class StudentException(Exception):
    pass

class Student:
    def __init__(self, id: int, name: str, attendance_count: int, grade: int):
        self.id = id
        self.name = name
        self.attendance_count = attendance_count
        self.grade = grade

    def print_string(self):
        """
        Used when printing in the UI
        @return: string to be printed
        """
        return "ID: " + str(self.id) + "| Name: " + self.name + "| Attendance count: " + str(self.attendance_count) + "| Grade: " + str(self.grade)

    def __str__(self):
        """
        Used when writing to file (formatting)
        @return: formatted string
        """
        return str(self.id) + ", " + self.name + ", " + str(self.attendance_count) + ", " + str(self.grade)

    def __eq__(self, other):
        if not isinstance(other, Student):
            raise StudentException("Cannot compare Student with " + str(type(other)))
        return self.id == other.id

    @property
    def id(self) -> int:
        return self.__id

    @id.setter
    def id(self, value: int):
        self.__id = value

    @property
    def name(self) -> str:
        return self.__name

    @name.setter
    def name(self, value: str):
        """
        A valid name is made of 2 words of at least 3 chars each
        @param value: Name of student
        """
        if len(value.split()) != 2:
            raise StudentException("Name must be made of 2 words")
        for word in value.split():
            if len(word) < 3:
                raise StudentException("Name must be made of 2 words of at least 3 chars each")
        self.__name = value

    @property
    def attendance_count(self) -> int:
        return self.__attendance_count

    @attendance_count.setter
    def attendance_count(self, value: int):
        if value < 0:
            raise StudentException("Attendance count must be a positive integer")
        self.__attendance_count = value

    @property
    def grade(self) -> int:
        return self.__grade

    @grade.setter
    def grade(self, value: int):
        if not 0 <= value <= 10:
            raise StudentException("Grade must be between 0 and 10")
        self.__grade = value

if __name__ == "__main__":
    alex = Student(1, "Alex Apa", 10, 10)
    string = str(alex)
    # parse the string to craete a student
    params = string.split(", ")
    print(params)
