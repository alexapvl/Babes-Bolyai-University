from src.domain.student import Student
from src.domain.discipline import Discipline
from src.domain.grade import Grade
from src.repository.students_register_management import StudentsRegisterRepository

class Service:
    def __init__(self, register: StudentsRegisterRepository):
        self.register = register

    # |-------------------------STATISTICS------------------------|

    def failing_students(self):
        """
        Generates a list with all the students for which the average grade at one or more disciplines is lower than 5.
        If a student has at least a grade at a discipline and the average grade is lower than 5, the student is added
        to the list.
        :return: a list of tuples consisting of (student, discipline, average grade for that discipline)
        """
        failing_students = []
        for student in self.register.list_students():
            for discipline in self.register.list_disciplines():
                average = self.student_average_grade_for_discipline(student, discipline)
                if average < 5 and self.student_enrolment_in_discipline(student, discipline):
                    failing_students.append((student, discipline, average))
        return failing_students

    def students_school_situation(self):
        """
        Generates a descending list of the students, based on their aggregate average grade.
        :return: a list of tuples consisting of (student, aggregate average grade)
        """
        situation = []
        for student in self.register.list_students():
            situation.append((student, self.student_aggregate_average_grade(student)))
        situation.sort(key=lambda x: x[1], reverse=True)
        return situation

    def disciplines_school_situation(self):
        """
        Generates a descending list of the disciplines, based on the average grade of all the
        students enrolled at that discipline.
        The discipline is added to the list if there are grades for it, if not, the discipline is skipped.
        :return: a list of tuples consisting of (discipline, average grade)
        """
        situation = []
        for discipline in self.register.list_disciplines():
            if self.discipline_enrolment(discipline):
                average = self.discipline_average_grade(discipline)
                situation.append((discipline, average))
        situation.sort(key=lambda x: x[1], reverse=True)
        return situation

    # |----------------------STUDENT METHODS----------------------|

    def student_average_grade_for_discipline(self, student: Student, discipline: Discipline):
        """
        Returns the average grade for a given student at a given discipline.
        :param student: The student for which the average grade is calculated
        :param discipline: The discipline for which the average grade is calculated
        :return: The average grade
        """
        grades = [grade.grade_value for grade in self.register.grades_list if grade.student_id == student.student_id and grade.discipline_id == discipline.discipline_id]
        if len(grades) == 0:
            return 0
        return sum(grades) / len(grades)

    def student_aggregate_average_grade(self, student: Student):
        """
        Returns the aggregate average grade for a given student.
        :param student: Student
        :return: The aggregate average
        """
        grades = [grade.grade_value for grade in self.register.grades_list if grade.student_id == student.student_id]
        if len(grades) == 0:
            return 0
        return sum(grades) / len(grades)

    def student_enrolment_in_discipline(self, student: Student, discipline: Discipline):
        """
        Checks if a student has at least a grade for a given discipline or not.
        If the student does not have any grades at that discipline, in means that
        the student is not enrolled at tht discipline.
        :param student: Student
        :param discipline: Discipline
        :return: True if the student is enrolled at the discipline, False otherwise
        """
        for grade in self.register.grades_list:
            if grade.student_id == student.student_id and grade.discipline_id == discipline.discipline_id:
                return True
        return False

    # |---------------------DISCIPLINE METHODS--------------------|

    def discipline_average_grade(self, discipline: Discipline):
        """
        Returns the average grade for a given discipline.
        :param discipline: Discipline
        :return: The average grade
        """
        grades = [grade.grade_value for grade in self.register.grades_list if grade.discipline_id == discipline.discipline_id]
        if len(grades) == 0:
            return 0
        return sum(grades) / len(grades)

    def discipline_enrolment(self, discipline: Discipline):
        """
        Returns the number of students enrolled at a given discipline.
        :param discipline: Discipline
        :return: True if there are students enrolled at the discipline, False otherwise
        """
        for grade in self.register.grades_list:
            if grade.discipline_id == discipline.discipline_id:
                return True
        return False

    # |---------------------REPOSITORY METHODS--------------------|
    # repo methods are commented in the repository file (students_register_management.py)

    def grade_student(self, grade: Grade):
        return self.register.grade_student(grade)

    def add_student(self, student: Student):
        return self.register.add_student(student)

    def remove_student(self, student: Student):
        return self.register.remove_student(student)

    def list_students(self):
        return self.register.list_students()

    def search_student_by_id(self, student_id: str):
        return self.register.search_student_by_id(student_id)

    def search_student_by_name(self, name: str):
        return self.register.search_student_by_name(name)

    def update_student(self, student: Student, new_student: Student):
        return self.register.update_student(student, new_student)

    def add_discipline(self, discipline: Discipline):
        return self.register.add_discipline(discipline)

    def remove_discipline(self, discipline: Discipline):
        return self.register.remove_discipline(discipline)

    def list_disciplines(self):
        return self.register.list_disciplines()

    def search_discipline_by_id(self, discipline_id: str):
        return self.register.search_discipline_by_id(discipline_id)

    def search_discipline_by_name(self, name: str):
        return self.register.search_discipline_by_name(name)

    def update_discipline(self, discipline: Discipline, new_discipline: Discipline):
        return self.register.update_discipline(discipline, new_discipline)
