from src.domain.student import Student
from src.domain.discipline import Discipline
from src.domain.grade import Grade
from src.repository.memory.students_memo_repo import StudentsMemoRepo
from src.repository.memory.disciplines_memo_repo import DisciplinesMemoRepo
from src.repository.memory.grades_memo_repo import GradesMemoRepo
from src.repository.text.students_text_repo import StudentsTextRepo
from src.repository.text.disciplines_text_repo import DisciplinesTextRepo
from src.repository.text.grades_text_repo import GradesTextRepo
from src.repository.binary.students_binary_repo import StudentsBinaryRepo
from src.repository.binary.disciplines_binary_repo import DisciplinesBinaryRepo
from src.repository.binary.grades_binary_repo import GradesBinaryRepo
from src.services.undo_redo_service import UndoRedoService, Operation, Command, CascadedOperation

class Service:
    def __init__(self, students_repo, disciplines_repo, grades_repo, undo_redo_service: UndoRedoService):
        self.students_repo = students_repo
        self.disciplines_repo = disciplines_repo
        self.grades_repo = grades_repo
        self.undo_redo_service = undo_redo_service

    # |-------------------------STATISTICS------------------------|

    def failing_students(self):
        """
        Generates a list with all the students for which the average grade at one or more disciplines is lower than 5.
        If a student has at least a grade at a discipline and the average grade is lower than 5, the student is added
        to the list.
        :return: a list of tuples consisting of (student, discipline, average grade for that discipline)
        """
        failing_students = []
        for student in self.students_repo.students:
            for discipline in self.disciplines_repo.disciplines:
                average = self.student_average_grade_for_discipline(student, discipline)
                if average < 5 and self.student_enrolment_in_discipline(student, discipline):
                    failing_students.append((student, discipline, average))
        return failing_students

    def students_school_situation(self):
        """
        Generates a descending list of the students, based on their aggregate average grade.
        :return: a list of tuples consisting of (student, aggregate average grade)
        """
        situation = []
        for student in self.students_repo.students:
            situation.append((student, self.student_aggregate_average_grade(student)))
        situation.sort(key=lambda x: x[1], reverse=True)
        return situation

    def disciplines_school_situation(self):
        """
        Generates a descending list of the disciplines, based on the average grade of all the
        students enrolled at that discipline.
        The discipline is added to the list if there are grades for it, if not, the discipline is skipped.
        :return: a list of tuples consisting of (discipline, average grade)
        """
        situation = []
        for discipline in self.disciplines_repo.disciplines:
            if self.discipline_enrolment(discipline):
                average = self.discipline_average_grade(discipline)
                situation.append((discipline, average))
        situation.sort(key=lambda x: x[1], reverse=True)
        return situation

    # |-------------------------UNDO/REDO-------------------------|

    def undo(self):
        self.undo_redo_service.undo()

    def redo(self):
        self.undo_redo_service.redo()

    # |----------------------STUDENT METHODS----------------------|

    def student_average_grade_for_discipline(self, student: Student, discipline: Discipline):
        """
        Returns the average grade for a given student at a given discipline.
        :param student: The student for which the average grade is calculated
        :param discipline: The discipline for which the average grade is calculated
        :return: The average grade
        """
        grades = [grade.value for grade in self.grades_repo.grades if grade.student_id == student.id and grade.discipline_id == discipline.id]
        if len(grades) == 0:
            return 0
        return sum(grades) / len(grades)

    def student_aggregate_average_grade(self, student: Student):
        """
        Returns the aggregate average grade for a given student.
        :param student: Student
        :return: The aggregate average
        """
        grades = [grade.value for grade in self.grades_repo.grades if grade.student_id == student.id]
        if len(grades) == 0:
            return 0
        return sum(grades) / len(grades)

    def student_enrolment_in_discipline(self, student: Student, discipline: Discipline):
        """
        Checks if a student has at least a grade for a given discipline or not.
        If the student does not have any grades at that discipline, in means that
        the student is not enrolled at tht discipline.
        :param student: Student
        :param discipline: Discipline
        :return: True if the student is enrolled at the discipline, False otherwise
        """
        for grade in self.grades_repo.grades:
            if grade.student_id == student.id and grade.discipline_id == discipline.id:
                return True
        return False

    # |---------------------DISCIPLINE METHODS--------------------|

    def discipline_average_grade(self, discipline: Discipline):
        """
        Returns the average grade for a given discipline.
        :param discipline: Discipline
        :return: The average grade
        """
        grades = [grade.value for grade in self.grades_repo.grades if grade.discipline_id == discipline.id]
        if len(grades) == 0:
            return 0
        return sum(grades) / len(grades)

    def discipline_enrolment(self, discipline: Discipline):
        """
        Returns the number of students enrolled at a given discipline.
        :param discipline: Discipline
        :return: True if there are students enrolled at the discipline, False otherwise
        """
        for grade in self.grades_repo.grades:
            if grade.discipline_id == discipline.id:
                return True
        return False

    # |---------------------REPOSITORY METHODS--------------------|

    def add_grade(self, grade: Grade):
        self.grades_repo.add_grade(grade)
        redo = Command(self.add_grade, grade)
        undo = Command(self.remove_grade, grade)
        self.undo_redo_service.record_for_undo(Operation(undo, redo))

    def remove_grade(self, grade: Grade):
        self.grades_repo.remove_grade(grade)
        redo = Command(self.remove_grade, grade)
        undo = Command(self.add_grade, grade)
        return Operation(undo, redo)

    def add_student(self, student: Student):
        self.students_repo.add_student(student)
        redo = Command(self.add_student, student)
        undo = Command(self.remove_student, student, self.grades_repo)
        self.undo_redo_service.record_for_undo(Operation(undo, redo))

    def remove_student(self, student: Student, grades_repo):
        redo = Command(self.remove_student, student, grades_repo)
        undo = Command(self.add_student, student)
        if grades_repo.search_grade_by_student_id(student):
            cascaded_ops = [Operation(undo, redo)]
            for grade in grades_repo.grades:
                if grade.student_id == student.id:
                    grade_operation = self.remove_grade(grade)
                    cascaded_ops.append(grade_operation)
            self.undo_redo_service.record_for_undo(CascadedOperation(cascaded_ops))
        else:
            self.undo_redo_service.record_for_undo(Operation(undo, redo))
        self.students_repo.remove_student(student)

    def update_student(self, student: Student, new_student: Student):
        student_copy = student.copy()
        new_student_copy = new_student.copy()
        redo = Command(self.update_student, student_copy, new_student_copy)
        undo = Command(self.update_student, new_student_copy, student_copy)
        self.undo_redo_service.record_for_undo(Operation(undo, redo))
        self.students_repo.update_student(self.students_repo.search_student_by_id(student.id), new_student_copy)

    def add_discipline(self, discipline: Discipline):
        self.disciplines_repo.add_discipline(discipline)
        redo = Command(self.add_discipline, discipline)
        undo = Command(self.remove_discipline, discipline, self.grades_repo)
        self.undo_redo_service.record_for_undo(Operation(undo, redo))

    def remove_discipline(self, discipline: Discipline, grades_repo):
        redo = Command(self.remove_discipline, discipline, grades_repo)
        undo = Command(self.add_discipline, discipline)
        cascaded_ops = [Operation(undo, redo)]
        for grade in grades_repo.grades:
            if grade.discipline_id == discipline.id:
                grade_operation = self.remove_grade(grade)
                cascaded_ops.append(grade_operation)
        self.undo_redo_service.record_for_undo(CascadedOperation(cascaded_ops))
        self.disciplines_repo.remove_discipline(discipline)

    def update_discipline(self, discipline: Discipline, new_discipline: Discipline):
        discipline_copy = discipline.copy()
        new_discipline_copy = new_discipline.copy()
        redo = Command(self.update_discipline, discipline_copy, new_discipline_copy)
        undo = Command(self.update_discipline, new_discipline_copy, discipline_copy)
        self.undo_redo_service.record_for_undo(Operation(undo, redo))
        self.disciplines_repo.update_discipline(self.disciplines_repo.search_discipline_by_id(discipline.id), new_discipline_copy)

    def list_students(self):
        return self.students_repo.students

    def search_student_by_id(self, student_id: str):
        return self.students_repo.search_student_by_id(student_id)

    def search_student_by_name(self, name: str):
        return self.students_repo.search_student_by_name(name)

    def list_disciplines(self):
        return self.disciplines_repo.disciplines

    def search_discipline_by_id(self, discipline_id: str):
        return self.disciplines_repo.search_discipline_by_id(discipline_id)

    def search_discipline_by_name(self, name: str):
        return self.disciplines_repo.search_discipline_by_name(name)

    # |------------------------PROPERTIES-------------------------|

    @property
    def students_repo(self):
        return self.__students_repo

    @students_repo.setter
    def students_repo(self, students_repo):
        if not isinstance(students_repo, StudentsMemoRepo) and not isinstance(students_repo, StudentsTextRepo) and not isinstance(students_repo, StudentsBinaryRepo):
            raise ValueError("Invalid students repository, must be of type StudentsRepo")
        self.__students_repo = students_repo

    @property
    def disciplines_repo(self):
        return self.__disciplines_repo

    @disciplines_repo.setter
    def disciplines_repo(self, disciplines_repo):
        if not isinstance(disciplines_repo, DisciplinesMemoRepo) and not isinstance(disciplines_repo, DisciplinesTextRepo) and not isinstance(disciplines_repo, DisciplinesBinaryRepo):
            raise ValueError("Invalid disciplines repository, must be of type DisciplinesRepo")
        self.__disciplines_repo = disciplines_repo

    @property
    def grades_repo(self):
        return self.__grades_repo

    @grades_repo.setter
    def grades_repo(self, grades_repo):
        if not isinstance(grades_repo, GradesMemoRepo) and not isinstance(grades_repo, GradesTextRepo) and not isinstance(grades_repo, GradesBinaryRepo):
            raise ValueError("Invalid grades repository, must be of type GradesRepo")
        self.__grades_repo = grades_repo
