from src.domain.discipline import Discipline
from unittest import TestCase

class TestDiscipline(TestCase):
    def test_discipline(self):
        discipline = Discipline("1", "Math")
        self.assertEqual(discipline.discipline_id, "1")
        self.assertEqual(discipline.name, "Math")

    def test_discipline_invalid_id(self):
        self.assertRaises(ValueError, Discipline, "", "Math")

    def test_discipline_invalid_name(self):
        self.assertRaises(ValueError, Discipline, "1", "")

    def test_discipline_validate_id(self):
        self.assertTrue(Discipline.validate_id("1"))
        self.assertFalse(Discipline.validate_id("a"))
        self.assertFalse(Discipline.validate_id("1a"))
        self.assertFalse(Discipline.validate_id("a1"))

    def test_discipline_validate_name(self):
        self.assertTrue(Discipline.validate_name("Math"))
        self.assertFalse(Discipline.validate_name("1"))
        self.assertFalse(Discipline.validate_name("Math 1"))
        self.assertFalse(Discipline.validate_name("1 Math"))
        self.assertFalse(Discipline.validate_name("1 1"))

    def test_discipline_eq(self):
        discipline1 = Discipline("1", "Math")
        discipline2 = Discipline("1", "Math")
        self.assertEqual(discipline1, discipline2)

    def test_discipline_ne(self):
        discipline1 = Discipline("1", "Math")
        discipline2 = Discipline("2", "Math")
        self.assertNotEqual(discipline1, discipline2)

    def test_discipline_str(self):
        discipline = Discipline("1", "Math")
        self.assertEqual(str(discipline), "ID: 1 | Name: Math")

    def test_discipline_update(self):
        discipline1 = Discipline("1", "Math")
        discipline2 = Discipline("1", "Mathematics")
        discipline1.update_discipline(discipline2)
        self.assertEqual(discipline1.name, "Mathematics")

    def test_discipline_update_invalid_object(self):
        discipline1 = Discipline("1", "Math")
        discipline2 = "Mathematics"
        self.assertRaises(ValueError, discipline1.update_discipline, discipline2)

    def test_random_discipline(self):
        discipline = Discipline.random_discipline()
        self.assertTrue(isinstance(discipline, Discipline))
