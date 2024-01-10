from src.domain.student import Student
from src.domain.discipline import Discipline
from src.domain.grade import Grade
from src.repository.binary.disciplines_binary_repo import DisciplinesBinaryRepo
from src.repository.binary.students_binary_repo import StudentsBinaryRepo
from src.repository.memory.grades_memo_repo import GradesMemoRepo
import pickle

class GradesBinaryRepo(GradesMemoRepo):
    def __init__(self, file_path: str):
        super().__init__()
        self.file_path = file_path

    # |----------------------FILE METHODS----------------------|

    def load_repo(self):
        try:
            with open(self.file_path, "rb") as file:
                while True:
                    grade = pickle.load(file)
                    self.grades.append(grade)
        except FileNotFoundError:
            self.grades = []
        except EOFError:
            pass

    def append_grade_to_repo(self, grade: Grade):
        if not isinstance(grade, Grade):
            raise ValueError("Invalid grade, must be of type Grade")
        with open(self.file_path, "ab") as file:
            pickle.dump(grade, file)

    def overwrite_repo(self):
        with open(self.file_path, "wb") as file:
            for grade in self.grades:
                pickle.dump(grade, file)

    # |--------------------REPOSITORY METHODS------------------|

    def add_grade(self, grade: Grade):
        super().add_grade(grade)
        self.append_grade_to_repo(grade)

    def remove_grade(self, grade: Grade):
        super().remove_grade(grade)
        self.overwrite_repo()

    @property
    def grades(self):
        return self.__grades

    @grades.setter
    def grades(self, grades):
        if not isinstance(grades, list):
            raise ValueError("Invalid grades, must be of type list")
        self.__grades = grades
