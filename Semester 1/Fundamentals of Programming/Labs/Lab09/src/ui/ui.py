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
from src.services.undo_redo_service import UndoRedoService, UndoRedoException
from src.services.services import Service
from colorama import Fore, Style
import configparser

class UI:
    def __init__(self, service: Service):
        self.service = service

    def run(self):
        try:
            while True:
                UI.menu()
                command = input(">>> ").strip()
                if command == "manage":
                    while True:
                        UI.menu_manage()
                        command = input(">>> ").strip()
                        try:
                            if command == "add_student" or command == "as":
                                while True:
                                    try:
                                        student_id = input("Student ID: ").strip()
                                        name = input("Name: ").strip()
                                        surname = input("Surname: ").strip()
                                        student = Student(student_id, name, surname)
                                        self.service.add_student(student)
                                        print(Fore.GREEN + "\nStudent added successfully\n" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "add_discipline" or command == "ad":
                                while True:
                                    try:
                                        discipline_id = input("Discipline ID: ").strip()
                                        name = input("Name: ").strip()
                                        discipline = Discipline(discipline_id, name)
                                        self.service.add_discipline(discipline)
                                        print(Fore.GREEN + "\nDiscipline added successfully\n" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "remove_student" or command == "rs":
                                while True:
                                    try:
                                        student_id = input("Student ID: ").strip()
                                        student = self.service.search_student_by_id(student_id)
                                        self.service.remove_student(student, self.service.grades_repo)
                                        print(Fore.GREEN + "\nStudent removed successfully\n" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "remove_discipline" or command == "rd":
                                while True:
                                    try:
                                        discipline_id = input("Discipline ID: ").strip()
                                        discipline = self.service.search_discipline_by_id(discipline_id)
                                        self.service.remove_discipline(discipline, self.service.grades_repo)
                                        print(Fore.GREEN + "\nDiscipline removed successfully\n" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "update_student" or command == "us":
                                while True:
                                    try:
                                        student_id = input("Student ID: ").strip()
                                        student = self.service.search_student_by_id(student_id)
                                        new_name = input("New name: ").strip()
                                        new_surname = input("New surname: ").strip()
                                        new_student = Student(student_id, new_name, new_surname)
                                        self.service.update_student(student, new_student)
                                        print(Fore.GREEN + "\nStudent updated successfully\n" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "update_discipline" or command == "ud":
                                while True:
                                    try:
                                        discipline_id = input("Discipline ID: ").strip()
                                        discipline = self.service.search_discipline_by_id(discipline_id)
                                        new_name = input("New name: ").strip()
                                        new_discipline = Discipline(discipline_id, new_name)
                                        self.service.update_discipline(discipline, new_discipline)
                                        print(Fore.GREEN + "\nDiscipline updated successfully\n" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "list_students" or command == "ls":
                                if len(self.service.list_students()) == 0:
                                    print(Fore.YELLOW + "\nWARNING: There are no students in the school\n" + Style.RESET_ALL)
                                else:
                                    for student in self.service.list_students():
                                        print(student)
                            elif command == "list_disciplines" or command == "ld":
                                if len(self.service.list_disciplines()) == 0:
                                    print(Fore.YELLOW + "\nWARNING: There are no disciplines in the school\n" + Style.RESET_ALL)
                                else:
                                    for discipline in self.service.list_disciplines():
                                        print(discipline)
                            elif command == "back":
                                break
                            else:
                                raise ValueError(Fore.RED + "\nInvalid command\n" + Style.RESET_ALL)
                        except ValueError as ve:
                            print(Fore.RED + f"{ve}" + Style.RESET_ALL)
                elif command == "grade":
                    while True:
                        try:
                            student_id = input("Student ID: ").strip()
                            discipline_id = input("Discipline ID: ").strip()
                            grade_value = float(input("Grade: ").strip())
                            student = self.service.search_student_by_id(student_id)
                            discipline = self.service.search_discipline_by_id(discipline_id)
                            grade = Grade(student, discipline, grade_value)
                            self.service.add_grade(grade)
                            print(Fore.GREEN + "\nStudent graded successfully\n" + Style.RESET_ALL)
                            break
                        except ValueError as ve:
                            print(Fore.RED + f"\n{ve}\n" + Style.RESET_ALL)
                elif command == "search":
                    while True:
                        try:
                            UI.menu_search()
                            command = input(">>> ").strip()
                            if command == "student_id" or command == "sid":
                                student_id = input("Student ID: ").strip()
                                student = self.service.search_student_by_id(student_id)
                                print(student)
                            elif command == "student_name" or command == "sn":
                                name = input("Name: ").strip()
                                matching_students = self.service.search_student_by_name(name)
                                if len(matching_students) == 0:
                                    print(Fore.YELLOW + "\nNo students found\n" + Style.RESET_ALL)
                                else:
                                    for student in matching_students:
                                        print(student)
                            elif command == "discipline_id" or command == "did":
                                discipline_id = input("Discipline ID: ").strip()
                                discipline = self.service.search_discipline_by_id(discipline_id)
                                print(discipline)
                            elif command == "discipline_name" or command == "dn":
                                name = input("Name: ").strip()
                                matching_disciplines = self.service.search_discipline_by_name(name)
                                if len(matching_disciplines) == 0:
                                    print(Fore.YELLOW + "\nNo disciplines found\n" + Style.RESET_ALL)
                                else:
                                    for discipline in matching_disciplines:
                                        print(discipline)
                            elif command == "back":
                                break
                            else:
                                raise ValueError(Fore.RED + "\nInvalid command\n" + Style.RESET_ALL)
                        except ValueError as ve:
                            print(Fore.RED + f"\n{ve}\n" + Style.RESET_ALL)
                elif command == "stats":
                    while True:
                        UI.menu_statistitcs()
                        command = input(">>> ").strip()
                        try:
                            if command == "list_failing" or command == "lf":
                                failing_students = self.service.failing_students()
                                if len(failing_students) == 0:
                                    print(Fore.YELLOW + "\nNo students are failing\n" + Style.RESET_ALL)
                                else:
                                    for situation in failing_students:
                                        student, discipline, average_grade = situation
                                        print(str(student) + " | " + str(discipline) + " | Average: " + str(average_grade))
                            elif command == "students_situation" or command == "ss":
                                students_school_situation = self.service.students_school_situation()
                                for situation in students_school_situation:
                                    student, aggregate_grade = situation
                                    print(str(student) + " | Aggregate: " + str(aggregate_grade))
                            elif command == "discipline_situation" or command == "ds":
                                disciplines_school_situation = self.service.disciplines_school_situation()
                                for situation in disciplines_school_situation:
                                    discipline, average_grade = situation
                                    print(str(discipline) + " | Average: " + str(average_grade))
                            elif command == "back":
                                break
                            else:
                                raise ValueError(Fore.RED + "\nInvalid command\n" + Style.RESET_ALL)
                        except ValueError as ve:
                            print(Fore.RED + f"\n{ve}\n" + Style.RESET_ALL)
                elif command == "undo":
                    try:
                        service.undo()
                        print(Fore.GREEN + "\nUndo successful\n" + Style.RESET_ALL)
                    except UndoRedoException as ure:
                        print(Fore.RED + f"\n{ure}\n" + Style.RESET_ALL)
                elif command == "redo":
                    try:
                        service.redo()
                        print(Fore.GREEN + "\nRedo successful\n" + Style.RESET_ALL)
                    except UndoRedoException as ure:
                        print(Fore.RED + f"\n{ure}\n" + Style.RESET_ALL)
                elif command == "exit":
                    print(Fore.MAGENTA + "\nBye!" + Style.RESET_ALL)
                    break
                else:
                    print(Fore.RED + "\nInvalid command\n" + Style.RESET_ALL)
        except ValueError as ve:
            print(Fore.RED + f"\n{ve}\n" + Style.RESET_ALL)

    @classmethod
    def get_config(cls) -> tuple:
        config = configparser.ConfigParser()
        config.read("../settings/settings.properties")
        repo_type = config.get("SettingsSection", "repository")
        students_path = config.get("SettingsSection", "students")
        disciplines_path = config.get("SettingsSection", "disciplines")
        grades_path = config.get("SettingsSection", "grades")
        return repo_type, students_path, disciplines_path, grades_path

    @classmethod
    def menu(cls):
        print(Fore.YELLOW + "manage." + Style.RESET_ALL + " Manage students and disciplines")
        print(Fore.YELLOW + "grade." + Style.RESET_ALL + " Grade student")
        print(Fore.YELLOW + "search." + Style.RESET_ALL + " Search(students/disciplines)")
        print(Fore.YELLOW + "stats." + Style.RESET_ALL + " Statistics")
        print(Fore.YELLOW + "undo." + Style.RESET_ALL + " Undo")
        print(Fore.YELLOW + "redo." + Style.RESET_ALL + " Redo")
        print(Fore.YELLOW + "exit." + Style.RESET_ALL + " Exit")

    @classmethod
    def menu_manage(cls):
        print(Fore.YELLOW + "add_student(as)." + Style.RESET_ALL + " Add student")
        print(Fore.YELLOW + "add_discipline(ad)." + Style.RESET_ALL + " Add discipline")
        print(Fore.RED + "remove_student(rs)." + Style.RESET_ALL + " Remove student")
        print(Fore.RED + "remove_discipline(rd)." + Style.RESET_ALL + " Remove discipline")
        print(Fore.CYAN + "update_student(us)." + Style.RESET_ALL + " Update student")
        print(Fore.CYAN + "update_discipline(ud)." + Style.RESET_ALL + " Update discipline")
        print(Fore.GREEN + "list_students(ls)." + Style.RESET_ALL + " List students")
        print(Fore.GREEN + "list_disciplines(ld)." + Style.RESET_ALL + " List disciplines")
        print(Fore.YELLOW + "back." + Style.RESET_ALL + " Back")

    @classmethod
    def menu_search(cls):
        print(Fore.YELLOW + "student_id(sid)." + Style.RESET_ALL + "Student - ID")
        print(Fore.YELLOW + "student_name(sn)." + Style.RESET_ALL + " Student - name(partial string matching)")
        print(Fore.YELLOW + "discipline_id(did)." + Style.RESET_ALL + " Discipline - ID")
        print(Fore.YELLOW + "discipline_name(dn)." + Style.RESET_ALL + " Discipline - name(partial string matching)")
        print(Fore.YELLOW + "back." + Style.RESET_ALL + " Back")

    @classmethod
    def menu_statistitcs(cls):
        print(Fore.YELLOW + "list_failing(lf)." + Style.RESET_ALL + " List failing students")
        print(Fore.YELLOW + "students_situation(ss)." + Style.RESET_ALL + " Show student situation")
        print(Fore.YELLOW + "discipline_situation(ds)." + Style.RESET_ALL + " Show discipline situation")
        print(Fore.YELLOW + "back." + Style.RESET_ALL + " Back")

if __name__ == "__main__":
    repository, students_path, disciplines_path, grades_path = UI.get_config()
    if repository == "Memory":
        print(Fore.MAGENTA + "Welcome to the Memory Repository" + Style.RESET_ALL)
        students_repo = StudentsMemoRepo()
        for student in StudentsMemoRepo.generate_20_random_students():
            students_repo.add_student(student)
        disciplines_repo = DisciplinesMemoRepo()
        for discipline in DisciplinesMemoRepo.generate_20_random_disciplines():
            disciplines_repo.add_discipline(discipline)
        grades_repo = GradesMemoRepo()
        for grade in GradesMemoRepo.generate_20_random_grades(students_repo.students, disciplines_repo.disciplines):
            grades_repo.add_grade(grade)
        undo_redo_service = UndoRedoService()
        service = Service(students_repo, disciplines_repo, grades_repo, undo_redo_service)
    elif repository == "Text":
        print(Fore.MAGENTA + "Welcome to the Text Repository" + Style.RESET_ALL)
        students_repo = StudentsTextRepo(students_path)
        students_repo.load_repo()
        disciplines_repo = DisciplinesTextRepo(disciplines_path)
        disciplines_repo.load_repo()
        grades_repo = GradesTextRepo(grades_path)
        grades_repo.load_repo()
        undo_redo_service = UndoRedoService()
        service = Service(students_repo, disciplines_repo, grades_repo, undo_redo_service)
    elif repository == "Binary":
        print(Fore.MAGENTA + "Welcome to the Binary Repository" + Style.RESET_ALL)
        students_repo = StudentsBinaryRepo(students_path)
        students_repo.load_repo()
        disciplines_repo = DisciplinesBinaryRepo(disciplines_path)
        disciplines_repo.load_repo()
        grades_repo = GradesBinaryRepo(grades_path)
        grades_repo.load_repo()
        undo_redo_service = UndoRedoService()
        service = Service(students_repo, disciplines_repo, grades_repo, undo_redo_service)
    else:
        raise ValueError(Fore.RED + "Invalid repository type" + Style.RESET_ALL)
    ui = UI(service)
    ui.run()
