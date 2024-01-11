from src.domain.student import Student
from src.repository.memory.students_memo_repo import StudentsMemoRepo

class StudentsTextRepo(StudentsMemoRepo):
    def __init__(self, file_path: str):
        super().__init__()
        self.file_path = file_path

    # |----------------------FILE METHODS----------------------|

    def load_repo(self):
        try:
            with open(self.file_path, "r") as file:
                lines = file.readlines()
                self.students = [StudentsTextRepo.unformat(line) for line in lines]
        except FileNotFoundError:
            self.students = []

    def append_student_to_repo(self, student: Student):
        if not isinstance(student, Student):
            raise ValueError("Invalid student, must be of type Student")
        with open(self.file_path, "a") as file:
            file.write(StudentsTextRepo.format(student) + "\n")

    def overwrite_repo(self):
        with open(self.file_path, "w") as file:
            for student in self.students:
                file.write(StudentsTextRepo.format(student) + "\n")

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

    @classmethod
    def format(cls, student: Student):
        """
        Creates a line of text from a student
        :param student: Student to be converted
        :return: String containing the student's ID and name(ID|name)
        """
        if not isinstance(student, Student):
            raise ValueError("Invalid student, must be of type Student")
        return student.id + "|" + student.name

    @classmethod
    def unformat(cls, line: str):
        """
        Creates a student from a line of text
        First splits the line by "|" to get the ID and name
        Then splits the name by " " to get the name and surname
        :param line: Line of text from the file
        :return: Student created from the line of text
        """
        if not isinstance(line, str):
            raise ValueError("Invalid line, must be of type str")
        line = line.strip()
        id_name = line.split("|")
        name = id_name[1].split()
        return Student(id_name[0], name[0], name[1])

    @property
    def file_path(self):
        return self.__file_path

    @file_path.setter
    def file_path(self, file_path: str):
        self.__file_path = file_path
