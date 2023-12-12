from src.domain.student import Student
from src.domain.discipline import Discipline
from src.domain.grade import Grade
from colorama import Fore, Style
from random import choice

class StudentsRegisterRepository:
    def __init__(self):
        self.students_list = StudentsRegisterRepository.generate_20_students()
        self.disciplines_list = StudentsRegisterRepository.generate_20_disciplines()
        self.grades_list = StudentsRegisterRepository.generate_20_grades(self.students_list, self.disciplines_list)

    # |----------------------GRADE METHODS----------------------|

    def grade_student(self, grade: Grade):
        """
        Adds a grade to the list of grades
        :param grade: Grade to be added
        """
        if not isinstance(grade, Grade):
            raise ValueError(Fore.RED + "Invalid grade, must be of type Grade!" + Style.RESET_ALL)
        self.grades_list.append(grade)

    # |---------------------STUDENT METHODS---------------------|

    def add_student(self, student: Student):
        """
        Adds a student to the list of students
        :param student: The student to be added
        """
        if student in self.students_list:
            raise ValueError(Fore.RED + "Student already exists!" + Style.RESET_ALL)
        self.students_list.append(student)

    def remove_student(self, student: Student):
        """
        Removes a student from the list of students by ID(__eq__ method)
        :param student: The student to be removed
        """
        if student not in self.students_list:
            raise ValueError(Fore.RED + "Student doesn't exist!" + Style.RESET_ALL)
        self.students_list.remove(student)
        # remove all grades for the student
        for grade in self.grades_list:
            if grade.student_id == student.student_id:
                self.grades_list.remove(grade)

    def list_students(self):
        """
        Returns the list of students
        :return: The list of students
        """
        return self.students_list

    def search_student_by_id(self, student_id: str):
        """
        Searches for a student by ID
        :param student_id: The ID of the student
        :return: Student with the given ID, if it exists
        """
        if not Student.validate_id(student_id):
            raise ValueError(Fore.RED + "Invalid ID, must contain only digits!" + Style.RESET_ALL)
        for student in self.students_list:
            if student.student_id == student_id:
                return student
        raise ValueError(Fore.RED + "Student with given ID doesn't exist!" + Style.RESET_ALL)

    def search_student_by_name(self, name: str):
        """
        Searches for a student by name
        :param name: The name of the student
        :return: Student with the given name, if it exists
        """
        matching_students = [student for student in self.students_list if name.lower() in student.name.lower()]
        if len(matching_students) == 0:
            raise ValueError(Fore.RED + "Student(s) with given name don't exist!" + Style.RESET_ALL)
        return matching_students

    def update_student(self, student: Student, new_student: Student):
        """
        Updates a student
        :param student: The student to be updated
        :param new_student: The new student
        :return:
        """
        if student not in self.students_list:
            raise ValueError(Fore.RED + "Student doesn't exist!" + Style.RESET_ALL)
        student.update_student(new_student)

    # |-------------------DISCIPLINE METHODS--------------------|

    def add_discipline(self, discipline: Discipline):
        """
        Adds a discipline to the list of disciplines
        :param discipline: Discipline to be added
        """
        if discipline in self.disciplines_list:
            raise ValueError(Fore.RED + "Discipline already exists!" + Style.RESET_ALL)
        self.disciplines_list.append(discipline)

    def remove_discipline(self, discipline: Discipline):
        """
        Removes a discipline from the list of disciplines by ID(__eq__ method)
        :param discipline: Discipline to be removed
        """
        if discipline not in self.disciplines_list:
            raise ValueError(Fore.RED + "Discipline doesn't exist!" + Style.RESET_ALL)
        self.disciplines_list.remove(discipline)
        # remove all grades for the discipline
        for grade in self.grades_list:
            if grade.discipline_id == discipline.discipline_id:
                self.grades_list.remove(grade)

    def list_disciplines(self):
        """
        Returns the list of disciplines
        :return: the list of disciplines
        """
        return self.disciplines_list

    def search_discipline_by_id(self, discipline_id: str):
        """
        Searches for a discipline by ID
        :param discipline_id: The ID of the discipline
        :return: The discipline with the given ID, if it exists
        """
        if not Discipline.validate_id(discipline_id):
            raise ValueError(Fore.RED + "Invalid ID, must contain only digits!" + Style.RESET_ALL)
        for discipline in self.disciplines_list:
            if discipline.discipline_id == discipline_id:
                return discipline
        raise ValueError(Fore.RED + "Discipline with given ID doesn't exist!" + Style.RESET_ALL)

    def search_discipline_by_name(self, name: str):
        """
        Searches for a discipline by name
        :param name: The name of the discipline
        :return: The discipline with the given name, if it exists
        """
        matching_disciplines = [discipline for discipline in self.disciplines_list if name.lower() in discipline.name.lower()]
        if len(matching_disciplines) == 0:
            raise ValueError(Fore.RED + "Discipline with given name doesn't exist!" + Style.RESET_ALL)
        return matching_disciplines

    def update_discipline(self, discipline: Discipline, new_discipline: Discipline):
        """
        Updates a discipline
        :param discipline: Discipline to be updated
        :param new_discipline: New discipline
        """
        if discipline not in self.disciplines_list:
            raise ValueError(Fore.RED + "Discipline doesn't exist!" + Style.RESET_ALL)
        discipline.update_discipline(new_discipline)

    @classmethod
    def generate_20_students(cls):
        """
        Generates 20 random students
        :return: A list of 20 random students
        """
        students = []
        while len(students) < 20:
            student = Student.random_student()
            if student not in students:
                students.append(student)
        return students

    @classmethod
    def generate_20_disciplines(cls):
        """
        Generates 20 random disciplines
        :return: A list of 20 random disciplines
        """
        disciplines = []
        while len(disciplines) < 20:
            discipline = Discipline.random_discipline()
            if discipline not in disciplines:
                disciplines.append(discipline)
        return disciplines

    @classmethod
    def generate_20_grades(cls, students: list, disciplines: list):
        """
        Generates 20 random grades
        :param students: A list of students from which to choose
        :param disciplines: A list of disciplines from which to choose
        :return: A list of 20 random grades
        """
        grades = []
        for _ in range(20):
            grade = Grade.random_grade(choice(students), choice(disciplines))
            grades.append(grade)
        return grades
