from src.domain.student import Student
from unittest import TestCase

class TestStudent(TestCase):
    def test_student(self):
        student = Student("1", "John", "Doe")
        self.assertEqual(student.student_id, "1")
        self.assertEqual(student.name, "John Doe")

    def test_student_invalid_id(self):
        self.assertRaises(ValueError, Student, "", "John", "Doe")

    def test_student_invalid_name(self):
        self.assertRaises(ValueError, Student, "1", "", "Doe")
        self.assertRaises(ValueError, Student, "1", "John", "")
        self.assertRaises(ValueError, Student, "1", "", "")

    def test_student_validate_id(self):
        self.assertTrue(Student.validate_id("1"))
        self.assertFalse(Student.validate_id("a"))
        self.assertFalse(Student.validate_id("1a"))
        self.assertFalse(Student.validate_id("a1"))

    def test_student_validate_name(self):
        self.assertTrue(Student.validate_name("John Doe"))
        self.assertFalse(Student.validate_name("John"))
        self.assertFalse(Student.validate_name("Doe"))
        self.assertFalse(Student.validate_name("John 1"))
        self.assertFalse(Student.validate_name("1 Doe"))
        self.assertFalse(Student.validate_name("1 1"))

    def test_update_student(self):
        student1 = Student("1", "John", "Doe")
        student2 = Student("1", "Johnny", "Smith")
        student1.update_student(student2)
        self.assertEqual(student1.name, "Johnny Smith")

    def test_update_student_invalid_object(self):
        student1 = Student("1", "John", "Doe")
        student2 = "Johnny Smith"
        self.assertRaises(ValueError, student1.update_student, student2)

    def test_student_eq(self):
        student1 = Student("1", "John", "Doe")
        student2 = Student("1", "Johnny", "Smith")
        self.assertEqual(student1, student2)

    def test_student_ne(self):
        student1 = Student("1", "John", "Doe")
        student2 = Student("2", "John", "Doe")
        self.assertNotEqual(student1, student2)

    def test_student_str(self):
        student = Student("1", "John", "Doe")
        self.assertEqual(str(student), "ID: 1 | Name: John Doe")
