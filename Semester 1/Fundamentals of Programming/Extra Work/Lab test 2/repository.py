from domain import Student

class RepositoryException(Exception):
    pass

class Repository:
    def __init__(self):
        self.students = []
        self.path = "students.txt"

    def load_file(self):
        with open(self.path, "r") as file:
            lines = file.readlines()
            for line in lines:
                line = line.strip()
                if line == "":
                    continue
                line = line.split(", ")
                student = Student(int(line[0]), line[1], int(line[2]), int(line[3]))
                self.students.append(student)

    def save_file(self):
        with open(self.path, "w") as file:
            for student in self.students:
                file.write(str(student) + "\n")

    def append_to_file(self, student: Student):
        with open(self.path, "a") as file:
            file.write(str(student) + "\n")

    def add_student(self, student: Student):
        if student in self.students:
            raise RepositoryException("Student with given ID already exists")
        self.students.append(student)
        self.append_to_file(student)

    @property
    def students(self):
        return self.__students

    @students.setter
    def students(self, students: list):
        self.__students = students
