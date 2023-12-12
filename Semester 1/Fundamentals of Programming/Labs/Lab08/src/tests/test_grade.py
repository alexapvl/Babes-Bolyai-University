from src.domain.student import Student
from src.domain.discipline import Discipline
from src.domain.grade import Grade
from unittest import TestCase

class TestGrade(TestCase):
    def test_grade(self):
        student = Student("1", "John", "Doe")
        discipline = Discipline("1", "Math")
        grade = Grade(student, discipline, 10)
        self.assertEqual(grade.student_id, "1")
        self.assertEqual(grade.discipline_id, "1")
        self.assertEqual(grade.grade_value, 10)

    def test_grade_invalid_grade_value(self):
        student = Student("1", "John", "Doe")
        discipline = Discipline("1", "Math")
        self.assertRaises(ValueError, Grade, student, discipline, -1)
        self.assertRaises(ValueError, Grade, student, discipline, 11)

    def test_grade_invalid_student(self):
        student = "John Doe"
        discipline = Discipline("1", "Math")
        self.assertRaises(ValueError, Grade, student, discipline, 10)

    def test_grade_invalid_discipline(self):
        student = Student("1", "John", "Doe")
        discipline = "Math"
        self.assertRaises(ValueError, Grade, student, discipline, 10)

    def test_grade_invalid_grade(self):
        student = Student("1", "John", "Doe")
        discipline = Discipline("1", "Math")
        self.assertRaises(ValueError, Grade, student, discipline, -1)
        self.assertRaises(ValueError, Grade, student, discipline, 11)

    def test_grade_validate_grade(self):
        self.assertTrue(Grade.validate_grade(0))
        self.assertTrue(Grade.validate_grade(10))
        self.assertTrue(Grade.validate_grade(5))
        self.assertFalse(Grade.validate_grade(-1))
        self.assertFalse(Grade.validate_grade(11))

    def test_grade_random_grade(self):
        student = Student("1", "John", "Doe")
        discipline = Discipline("1", "Math")
        grade = Grade.random_grade(student, discipline)
        self.assertTrue(isinstance(grade, Grade))

    def test_grade_str(self):
        student = Student("1", "John", "Doe")
        discipline = Discipline("1", "Math")
        grade = Grade(student, discipline, 10)
        self.assertEqual(str(grade), "Student: 1 | Discipline: 1 | Grade: 10")
