from repository import Repository

class Service:
    def __init__(self, repo: Repository):
        self.__repo = repo

    def add(self, student):
        """
        Adds a student to the repo
        @param student: Student to be added
        """
        self.__repo.add_student(student)

    def get_students_decreasing_order_based_on_grade(self):
        return sorted(self.__repo.students, key=lambda student: (-student.grade, student.name))

    def give_bonuses(self, p: int, b: int):
        for student in self.__repo.students:
            if student.attendance_count >= p:
                if student.grade + b > 10:
                    student.grade = 10
                else:
                    student.grade += b
                self.__repo.save_file()

    def display_students_by_given_string(self, string: str):
        """
        Displays all students whose name contains a given string
        @param string: String to be searched for
        @return List of students whose name contains the given string
        """
        return sorted([student for student in self.__repo.students if string.lower() in student.name.lower()], key=lambda student: student.name)
