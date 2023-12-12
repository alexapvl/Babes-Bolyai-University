from src.domain.student import Student
from src.domain.discipline import Discipline
from src.domain.grade import Grade
from src.repository.students_register_management import StudentsRegisterRepository
from unittest import TestCase


class TestRepository(TestCase):

    # |----------------------GRADE METHODS-----------------------|

    def test_grade_student(self):
        repo = StudentsRegisterRepository()
        student = Student("1", "John", "Doe")
        discipline = Discipline("1", "Math")
        grade = Grade(student, discipline, 10)
        repo.grade_student(grade)
        self.assertEqual(len(repo.grades_list), 21)

    # |---------------------STUDENT METHODS---------------------|

    def test_add_student(self):
        repo = StudentsRegisterRepository()
        student = Student("1", "John", "Doe")
        repo.add_student(student)
        self.assertEqual(len(repo.students_list), 21)

    def test_remove_student(self):
        repo = StudentsRegisterRepository()
        student = Student("1", "John", "Doe")
        repo.add_student(student)
        repo.remove_student(student)
        self.assertEqual(len(repo.students_list), 20)

    def test_list_students(self):
        repo = StudentsRegisterRepository()
        self.assertEqual(len(repo.list_students()), 20)

    def test_search_student_by_id(self):
        repo = StudentsRegisterRepository()
        student = Student("1", "John", "Doe")
        repo.add_student(student)
        self.assertEqual(repo.search_student_by_id("1"), student)

    def test_search_student_by_id_not_found(self):
        repo = StudentsRegisterRepository()
        self.assertRaises(ValueError, repo.search_student_by_id, "2")

    def test_search_student_by_name(self):
        repo = StudentsRegisterRepository()
        student = Student("1", "John", "Doe")
        repo.add_student(student)
        self.assertIn(student, repo.search_student_by_name("John"))

    def test_search_student_by_name_not_found(self):
        repo = StudentsRegisterRepository()
        self.assertRaises(ValueError, repo.search_student_by_name, "Johnnyyyyyy")

    def test_update_student(self):
        repo = StudentsRegisterRepository()
        student = Student("1", "John", "Doe")
        repo.add_student(student)
        new_student = Student("1", "John", "Smith")
        repo.update_student(student, new_student)
        self.assertEqual(repo.students_list[20].name, "John Smith")

    def test_update_student_not_found(self):
        repo = StudentsRegisterRepository()
        student = Student("1", "John", "Doe")
        new_student = Student("1", "John", "Smith")
        self.assertRaises(ValueError, repo.update_student, student, new_student)

    # |-------------------DISCIPLINE METHODS--------------------|

    def test_add_discipline(self):
        repo = StudentsRegisterRepository()
        discipline = Discipline("1", "Math")
        repo.add_discipline(discipline)
        self.assertEqual(len(repo.disciplines_list), 21)

    def test_add_discipline_already_exists(self):
        repo = StudentsRegisterRepository()
        discipline = Discipline("1", "Math")
        repo.add_discipline(discipline)
        self.assertRaises(ValueError, repo.add_discipline, discipline)

    def test_remove_discipline(self):
        repo = StudentsRegisterRepository()
        discipline = Discipline("1", "Math")
        repo.add_discipline(discipline)
        repo.remove_discipline(discipline)
        self.assertEqual(len(repo.disciplines_list), 20)

    def test_remove_discipline_not_found(self):
        repo = StudentsRegisterRepository()
        discipline = Discipline("1", "Math")
        self.assertRaises(ValueError, repo.remove_discipline, discipline)

    def test_list_disciplines(self):
        repo = StudentsRegisterRepository()
        self.assertEqual(len(repo.list_disciplines()), 20)

    def test_search_discipline_by_id(self):
        repo = StudentsRegisterRepository()
        discipline = Discipline("1", "Math")
        repo.add_discipline(discipline)
        self.assertEqual(repo.search_discipline_by_id("1"), discipline)

    def test_search_discipline_by_id_not_found(self):
        repo = StudentsRegisterRepository()
        self.assertRaises(ValueError, repo.search_discipline_by_id, "2")

    def test_search_discipline_by_name(self):
        repo = StudentsRegisterRepository()
        discipline = Discipline("1", "Math")
        repo.add_discipline(discipline)
        self.assertIn(discipline, repo.search_discipline_by_name("Math"))

    def test_search_discipline_by_name_not_found(self):
        repo = StudentsRegisterRepository()
        self.assertRaises(ValueError, repo.search_discipline_by_name, "Math")

    def test_update_discipline(self):
        repo = StudentsRegisterRepository()
        discipline = Discipline("1", "Fundamentals")
        repo.add_discipline(discipline)
        new_discipline = Discipline("1", "Fundamentals of Programming")
        repo.update_discipline(discipline, new_discipline)
        self.assertEqual(repo.disciplines_list[20].name, "Fundamentals of Programming")

    def test_update_discipline_not_found(self):
        repo = StudentsRegisterRepository()
        discipline = Discipline("1", "Fundamentals")
        new_discipline = Discipline("1", "Fundamentals of Programming")
        self.assertRaises(ValueError, repo.update_discipline, discipline, new_discipline)

    # |----------------CLASS REPOSITORY METHODS-----------------|

    def test_generate_students(self):
        self.assertEqual(len(StudentsRegisterRepository.generate_20_students()), 20)

    def test_generate_disciplines(self):
        self.assertEqual(len(StudentsRegisterRepository.generate_20_disciplines()), 20)

    def test_generate_grades(self):
        repo = StudentsRegisterRepository()
        self.assertEqual(len(StudentsRegisterRepository.generate_20_grades(repo.students_list, repo.disciplines_list)), 20)
