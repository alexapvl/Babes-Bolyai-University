from src.domain.student import Student
from src.domain.discipline import Discipline
from src.domain.grade import Grade
from random import choice

class GradesMemoRepo:
    def __init__(self):
        self.grades = []

    def add_grade(self, grade: Grade):
        """
        Adds a grade to the list of grades
        :param grade: Grade to be added
        """
        if not isinstance(grade, Grade):
            raise ValueError("Invalid grade, must be of type Grade")
        self.grades.append(grade)

    def remove_grade(self, grade: Grade):
        """
        Removes only ONE grade from the list of grades
        :param grade: Grade to be removed
        """
        if not isinstance(grade, Grade):
            raise ValueError("Invalid grade, must be of type Grade")
        for g in self.grades:
            if g.student_id == grade.student_id and g.discipline_id == grade.discipline_id and g.value == grade.value:
                self.grades.remove(g)
                break

    def search_grade_by_student_id(self, student: Student):
        """
        Searches for a grade by student ID
        :param student: Student whose grades are to be searched
        :return: A list of grades for the given student
        """
        if not isinstance(student, Student):
            raise ValueError("Invalid student, must be of type Student")
        matching_grades = [grade for grade in self.grades if grade.student_id == student.id]
        if len(matching_grades) == 0:
            return False
        return True

    @property
    def grades(self):
        return self.__grades

    @grades.setter
    def grades(self, grades_list: list):
        self.__grades = grades_list

    @classmethod
    def generate_20_random_grades(cls, students: list, disciplines: list):
        """
        Generates 20 random grades
        :param students: A list of students from which to choose
        :param disciplines: A list of disciplines from which to choose
        :return: A list of 20 random grades
        """
        grades_generated_list = []
        for _ in range(20):
            grade = Grade.random_grade(choice(students), choice(disciplines))
            grades_generated_list.append(grade)
        return grades_generated_list
