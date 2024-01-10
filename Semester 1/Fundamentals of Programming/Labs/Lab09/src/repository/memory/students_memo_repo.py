from src.domain.student import Student

class StudentsMemoRepo:
    def __init__(self):
        self.students = []

    def add_student(self, student: Student):
        """
        Adds a student to the list of students
        :param student: The student to be added
        """
        if student in self.students:
            raise ValueError("Student already exists")
        self.students.append(student)

    def remove_student(self, student: Student):
        """
        Removes a student from the list of students by ID(__eq__ method)
        :param student: The student to be removed
        """
        if student not in self.students:
            raise ValueError("Student doesn't exist")
        self.students.remove(student)

    def search_student_by_id(self, student_id: str):
        """
        Searches for a student by ID
        :param student_id: The ID of the student
        :return: Student with the given ID, if it exists
        """
        if not Student.validate_id(student_id):
            raise ValueError("Invalid ID, must contain only digits")
        for student in self.students:
            if student.id == student_id:
                return student
        raise ValueError("Student with given ID doesn't exist")

    def search_student_by_name(self, name: str):
        """
        Searches for a student by name
        :param name: The name of the student
        :return: Student with the given name, if it exists
        """
        matching_students = [student for student in self.students if name.lower() in student.name.lower()]
        if len(matching_students) == 0:
            raise ValueError("Student(s) with given name don't exist")
        return matching_students

    def update_student(self, student: Student, new_student: Student):
        """
        Updates a student
        :param student: The student to be updated
        :param new_student: The new student
        """
        if student not in self.students:
            raise ValueError("Student doesn't exist")
        student.update_student(new_student)

    @property
    def students(self):
        return self.__students

    @students.setter
    def students(self, students: list):
        self.__students = students

    @classmethod
    def generate_20_random_students(cls):
        """
        Generates 20 random students
        :return: A list of 20 random students
        """
        students_generated_list = []
        while len(students_generated_list) < 20:
            student = Student.random_student()
            if student not in students_generated_list:
                students_generated_list.append(student)
        return students_generated_list
