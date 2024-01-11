from src.domain.student import Student
from src.domain.discipline import Discipline
from random import uniform

class Grade:
    def __init__(self, student: Student, discipline: Discipline, value: float):
        self.student_id = student
        self.discipline_id = discipline
        self.value = value

    @property
    def student_id(self):
        return self.__student_id

    @student_id.setter
    def student_id(self, student: Student):
        if not isinstance(student, Student):
            raise ValueError("Invalid student, must be of type Student")
        self.__student_id = student.id

    @property
    def discipline_id(self):
        return self.__discipline_id

    @discipline_id.setter
    def discipline_id(self, discipline: Discipline):
        if not isinstance(discipline, Discipline):
            raise ValueError("Invalid discipline, must be of type Discipline")
        self.__discipline_id = discipline.id

    @property
    def value(self):
        return self.__value

    @value.setter
    def value(self, grade: float):
        if not self.validate_grade(grade):
            raise ValueError("Invalid grade, must be between 0 and 10")
        self.__value = grade

    @classmethod
    def validate_grade(cls, grade: float):
        """
        Validates a grade.
        A grade needs to be between 0 and 10
        :param grade: Grade to be validated
        :return: True if the grade is valid, False otherwise
        """
        if 0 <= grade <= 10:
            return True
        return False

    @classmethod
    def random_grade(cls, student: Student, discipline: Discipline):
        """
        Generates a random grade for a student and a discipline.
        The value of the grade is float between 0 and 10
        :param student: Student for which the grade is generated
        :param discipline: Discipline at which the student received the grade
        :return: The grade object
        """
        grade = Grade(student, discipline, round(uniform(0, 10), 2))
        return grade

    def __str__(self):
        return "Student: " + self.student_id + " | Discipline: " + self.discipline_id + " | Grade: " + str(self.value)

    def __eq__(self, other):
        if not isinstance(other, Grade):
            raise ValueError("Invalid grade, must be of type Grade")
        return self.student_id == other.student_id and self.discipline_id == other.discipline_id and self.value == other.value
