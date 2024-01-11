from src.domain.student import Student
from src.repository.memory.students_memo_repo import StudentsMemoRepo
import pickle

class StudentsBinaryRepo(StudentsMemoRepo):
    def __init__(self, file_path: str):
        super().__init__()
        self.file_path = file_path

    # |----------------------FILE METHODS----------------------|

    def load_repo(self):
        try:
            with open(self.file_path, "rb") as file:
                while True:
                    student = pickle.load(file)
                    self.students.append(student)
        except FileNotFoundError:
            self.students = []
        except EOFError:
            pass

    def append_student_to_repo(self, student: Student):
        if not isinstance(student, Student):
            raise ValueError("Invalid student, must be of type Student")
        with open(self.file_path, "ab") as file:
            pickle.dump(student, file)

    def overwrite_repo(self):
        with open(self.file_path, "wb") as file:
            for student in self.students:
                pickle.dump(student, file)

    # |--------------------REPOSITORY METHODS------------------|

    def add_student(self, student: Student):
        super().add_student(student)
        self.append_student_to_repo(student)

    def remove_student(self, student: Student):
        super().remove_student(student)
        self.overwrite_repo()

    def update_student(self, student: Student, new_student: Student):
        super().update_student(student, new_student)
        self.overwrite_repo()

    @property
    def file_path(self):
        return self.__file_path

    @file_path.setter
    def file_path(self, file_path):
        self.__file_path = file_path
