from src.domain.discipline import Discipline
from src.domain.student import Student
from src.domain.grade import Grade
from src.repository.memory.grades_memo_repo import GradesMemoRepo


class GradesTextRepo(GradesMemoRepo):
    def __init__(self, file_path: str):
        super().__init__()
        self.file_path = file_path

    # |----------------------FILE METHODS----------------------|

    def load_repo(self):
        try:
            with open(self.file_path, "r") as file:
                lines = file.readlines()
                self.grades = [GradesTextRepo.unformat(line) for line in lines]
        except FileNotFoundError:
            self.grades = []

    def append_grade_to_repo(self, grade: Grade):
        if not isinstance(grade, Grade):
            raise ValueError("Invalid grade, must be of type Grade")
        with open(self.file_path, "a") as file:
            file.write(GradesTextRepo.format(grade) + "\n")

    def overwrite_repo(self):
        with open(self.file_path, "w") as file:
            for grade in self.grades:
                file.write(GradesTextRepo.format(grade) + "\n")

    # |--------------------REPOSITORY METHODS------------------|

    def add_grade(self, grade: Grade):
        super().add_grade(grade)
        self.append_grade_to_repo(grade)

    def remove_grade(self, grade: Grade):
        super().remove_grade(grade)
        self.overwrite_repo()

    @classmethod
    def format(cls, grade: Grade):
        """
        Creates a line of text from a grade
        :param grade: Grade to be converted
        :return: String containing the grade's ID, student ID, discipline ID and value(ID|student_id|discipline_id|value)
        """
        if not isinstance(grade, Grade):
            raise ValueError("Invalid grade, must be of type Grade")
        return grade.student_id + "|" + grade.discipline_id + "|" + str(grade.value)

    @classmethod
    def unformat(cls, line: str):
        """
        Creates a grade from a line of text
        First splits the line by "|" to get the ID, student ID, discipline ID and value
        :param line: Line of text from the file
        :return: Grade created from the line of text
        """
        if not isinstance(line, str):
            raise ValueError("Invalid line, must be of type str")
        grade = line.split("|")
        temp_student = Student(grade[0], "Temporary", "Temporary")
        temp_discipline = Discipline(grade[1], "Temporary")
        # we declare the student and discipline as temporary because we don't know their names,
        # so we only care about their IDs, so we put random valid names which will not be used
        # OBS: When calling the init for Grade, the first two args are objects of type Student and Discipline
        # that's why we need to create temporary objects of those types
        return Grade(temp_student, temp_discipline, float(grade[2]))

    @property
    def file_path(self):
        return self.__file_path

    @file_path.setter
    def file_path(self, file_path: str):
        if not isinstance(file_path, str):
            raise ValueError("Invalid file path, must be of type str")
        self.__file_path = file_path
