from colorama import Fore, Style
import re
from random import randint, choice

class Student:
    def __init__(self, id: str, name: str, surname: str):
        self.id = id
        self.name = name + " " + surname

    @property
    def id(self):
        return self.__student_id

    @id.setter
    def id(self, id: str):
        if not Student.validate_id(id):
            raise ValueError(Fore.RED + "Invalid id, must contain only digits" + Style.RESET_ALL)
        self.__student_id = id

    @property
    def name(self):
        return self.__name

    @name.setter
    def name(self, name: str):
        if not Student.validate_name(name):
            raise ValueError("Invalid name, must start with a capital letter and contain only letters\nName and Surname start with uppercase letters")
        self.__name = name

    @classmethod
    def validate_id(cls, student_id: str):
        """
        Validates a student id using regex.
        An ID should only contain digits
        :param student_id: the string to be validated
        :return: True if the id is valid, False otherwise
        """
        if re.match("^[0-9]+$", student_id):
            return True
        return False

    @classmethod
    def validate_name(cls, name: str):
        """
        Validates a name using regex.
        A name should start with a capital letter and contain only letters
        :param name: The name to be validated
        :return: True if the name is valid, False otherwise
        """
        if re.match("^[A-Z][a-z]+ [A-Z][a-z]+$", name):
            return True
        return False

    @classmethod
    def random_student(cls):
        """
        Generates a random student from a given data set.
        The ID is also randomly generated using randint
        :return: The student object
        """
        id = str(randint(100, 999))
        names = [
            'Riley', 'Chloe', 'Liam', 'Ava', 'Ethan', 'Scarlett', 'Jackson', 'Emma', 'Mason', 'Harper',
            'Lucas', 'Mia', 'Noah', 'Isabella', 'Aiden', 'Olivia', 'Logan', 'Sophia', 'Caleb', 'Addison',
            'Wyatt', 'Grace', 'Elijah', 'Lily', 'Benjamin', 'Stella', 'Oliver', 'Abigail', 'Henry', 'Charlotte',
            'Carter', 'Zoey', 'Gabriel', 'Riley', 'Chloe', 'Daniel', 'Emily', 'Samuel', 'Natalie', 'Alexander',
            'Layla', 'William', 'Penelope', 'Owen', 'Victoria', 'Zoe', 'Mason', 'Ella', 'Noah', 'Liam', 'Zoey',
            'Logan', 'Natalie', 'Olivia', 'Wyatt', 'Emily', 'Leo', 'Stella', 'Liam', 'Alexander', 'Riley', 'Harper',
            'Aiden', 'Abigail', 'Jackson', 'Amelia', 'Caleb', 'Scarlett', 'Ethan', 'Addison', 'Henry', 'Chloe',
            'Wyatt', 'Emily', 'Samuel', 'Lily', 'Benjamin', 'Zoey', 'Elijah', 'Grace', 'Carter', 'Stella', 'Daniel',
            'Riley', 'Ava', 'Gabriel', 'Charlotte', 'Logan', 'Olivia', 'Noah', 'Sophia', 'William', 'Layla', 'Owen',
            'Natalie', 'Leo', 'Mia', 'Alexander', 'Isabella', 'Lucas'
        ]
        surnames = [
            'Smith', 'Johnson', 'Williams', 'Jones', 'Brown', 'Davis', 'Miller', 'Wilson', 'Moore', 'Taylor',
            'Anderson', 'Thomas', 'Jackson', 'White', 'Harris', 'Martin', 'Thompson', 'Garcia', 'Martinez', 'Robinson',
            'Clark', 'Rodriguez', 'Lewis', 'Lee', 'Walker', 'Hall', 'Allen', 'Young', 'Hernandez', 'King', 'Wright',
            'Lopez', 'Hill', 'Scott', 'Green', 'Adams', 'Baker', 'Gonzalez', 'Nelson', 'Carter', 'Mitchell', 'Perez',
            'Roberts', 'Turner', 'Phillips', 'Campbell', 'Parker', 'Evans', 'Edwards', 'Collins', 'Stewart', 'Sanchez',
            'Morris', 'Rogers', 'Reed', 'Cook', 'Morgan', 'Bell', 'Murphy', 'Bailey', 'Rivera', 'Cooper', 'Richardson',
            'Cox', 'Howard', 'Ward', 'Torres', 'Peterson', 'Gray', 'Ramirez', 'James', 'Watson', 'Brooks', 'Kelly',
            'Sanders', 'Price', 'Bennett', 'Wood', 'Barnes', 'Ross', 'Henderson', 'Coleman', 'Jenkins', 'Perry', 'Long',
            'Foster', 'Reynolds', 'Patterson', 'Hughes', 'Hayes', 'Simmons', 'Fisher', 'Ford', 'Marshall', 'Owens'
        ]
        name = choice(names)
        surname = choice(surnames)
        return Student(id, name, surname)

    def update_student(self, new_student):
        """
        Updates the student's name.
        ID remains the same
        :param new_student: The new student
        :return: The updated student
        """
        if not isinstance(new_student, Student):
            raise ValueError("Invalid student, must be of type Student")
        self.name = new_student.name

    def copy(self):
        """
        Creates a copy of the student
        :return: returns a copy of the student
        """
        return Student(self.id, self.name.split()[0], self.name.split()[1])

    def __str__(self):
        return "ID: " + self.id + " | Name: " + self.name

    def __eq__(self, other):
        if not isinstance(other, Student):
            raise ValueError("Can't compare Student with other type")
        return self.id == other.id
