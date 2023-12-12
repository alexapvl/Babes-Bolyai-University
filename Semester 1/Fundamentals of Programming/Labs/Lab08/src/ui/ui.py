from src.domain.student import Student
from src.domain.discipline import Discipline
from src.domain.grade import Grade
from src.repository.students_register_management import StudentsRegisterRepository
from src.services.services import Service
from colorama import Fore, Style

class UI:
    def __init__(self, repo: Service):
        self.service = service

    def run(self):
        try:
            while True:
                UI.menu()
                command = input(">>> ").strip()
                if command == "1":
                    while True:
                        UI.menu_manage()
                        command = input(">>> ").strip()
                        try:
                            if command == "1":
                                while True:
                                    try:
                                        student_id = input("Student ID: ").strip()
                                        name = input("Name: ").strip()
                                        surname = input("Surname: ").strip()
                                        student = Student(student_id, name, surname)
                                        self.service.add_student(student)
                                        print(Fore.GREEN + "Student added successfully" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "2":
                                while True:
                                    try:
                                        discipline_id = input("Discipline ID: ").strip()
                                        name = input("Name: ").strip()
                                        discipline = Discipline(discipline_id, name)
                                        self.service.add_discipline(discipline)
                                        print(Fore.GREEN + "Discipline added successfully" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "3":
                                while True:
                                    try:
                                        student_id = input("Student ID: ").strip()
                                        student = self.service.search_student_by_id(student_id)
                                        self.service.remove_student(student)
                                        print(Fore.GREEN + "Student removed successfully" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "4":
                                while True:
                                    try:
                                        discipline_id = input("Discipline ID: ").strip()
                                        discipline = self.service.search_discipline_by_id(discipline_id)
                                        self.service.remove_discipline(discipline)
                                        print(Fore.GREEN + "Discipline removed successfully" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                                print(Fore.GREEN + "Discipline removed successfully" + Style.RESET_ALL)
                            elif command == "5":
                                while True:
                                    try:
                                        student_id = input("Student ID: ").strip()
                                        student = self.service.search_student_by_id(student_id)
                                        new_name = input("New name: ").strip()
                                        new_surname = input("New surname: ").strip()
                                        new_student = Student("000", new_name, new_surname)
                                        self.service.update_student(student, new_student)
                                        print(Fore.GREEN + "Student updated successfully" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "6":
                                while True:
                                    try:
                                        discipline_id = input("Discipline ID: ").strip()
                                        discipline = self.service.search_discipline_by_id(discipline_id)
                                        new_name = input("New name: ").strip()
                                        new_discipline = Discipline("000", new_name)
                                        self.service.update_discipline(discipline, new_discipline)
                                        print(Fore.GREEN + "Discipline updated successfully" + Style.RESET_ALL)
                                        break
                                    except ValueError as ve:
                                        print(ve)
                            elif command == "7":
                                if len(self.service.list_students()) == 0:
                                    print(Fore.YELLOW + "WARNING: There are no students in the school" + Style.RESET_ALL)
                                else:
                                    for student in self.service.list_students():
                                        print(student)
                            elif command == "8":
                                if len(self.service.list_disciplines()) == 0:
                                    print(Fore.YELLOW + "WARNING: There are no disciplines in the school" + Style.RESET_ALL)
                                else:
                                    for discipline in self.service.list_disciplines():
                                        print(discipline)
                            elif command == "0":
                                break
                            else:
                                raise ValueError(Fore.RED + "Invalid command" + Style.RESET_ALL)
                        except ValueError as ve:
                            print(Fore.RED + f"{ve}" + Style.RESET_ALL)
                elif command == "2":
                    while True:
                        try:
                            student_id = input("Student ID: ").strip()
                            discipline_id = input("Discipline ID: ").strip()
                            grade_value = float(input("Grade: ").strip())
                            student = self.service.search_student_by_id(student_id)
                            discipline = self.service.search_discipline_by_id(discipline_id)
                            grade = Grade(student, discipline, grade_value)
                            self.service.grade_student(grade)
                            print(Fore.GREEN + "Student graded successfully" + Style.RESET_ALL)
                            break
                        except ValueError as ve:
                            print(Fore.RED + f"{ve}" + Style.RESET_ALL)
                elif command == "3":
                    while True:
                        try:
                            UI.menu_search()
                            command = input(">>> ").strip()
                            if command == "1":
                                student_id = input("Student ID: ").strip()
                                student = self.service.search_student_by_id(student_id)
                                print(student)
                            elif command == "2":
                                name = input("Name: ").strip()
                                matching_students = self.service.search_student_by_name(name)
                                if len(matching_students) == 0:
                                    print(Fore.YELLOW + "No students found" + Style.RESET_ALL)
                                else:
                                    for student in matching_students:
                                        print(student)
                            elif command == "3":
                                discipline_id = input("Discipline ID: ").strip()
                                discipline = self.service.search_discipline_by_id(discipline_id)
                                print(discipline)
                            elif command == "4":
                                name = input("Name: ").strip()
                                matching_disciplines = self.service.search_discipline_by_name(name)
                                if len(matching_disciplines) == 0:
                                    print(Fore.YELLOW + "No disciplines found" + Style.RESET_ALL)
                                else:
                                    for discipline in matching_disciplines:
                                        print(discipline)
                            elif command == "0":
                                break
                            else:
                                raise ValueError(Fore.RED + "Invalid command" + Style.RESET_ALL)
                        except ValueError as ve:
                            print(Fore.RED + f"{ve}" + Style.RESET_ALL)
                elif command == "4":
                    while True:
                        UI.menu_statistitcs()
                        command = input(">>> ").strip()
                        try:
                            if command == "1":
                                failing_students = self.service.failing_students()
                                if len(failing_students) == 0:
                                    print(Fore.YELLOW + "No students are failing" + Style.RESET_ALL)
                                else:
                                    for situation in failing_students:
                                        student, discipline, average_grade = situation
                                        print(str(student) + " | " + str(discipline) + " | Average: " + str(average_grade))
                            elif command == "2":
                                students_school_situation = self.service.students_school_situation()
                                for situation in students_school_situation:
                                    student, aggregate_grade = situation
                                    print(str(student) + " | Aggregate: " + str(aggregate_grade))
                            elif command == "3":
                                disciplines_school_situation = self.service.disciplines_school_situation()
                                for situation in disciplines_school_situation:
                                    discipline, average_grade = situation
                                    print(str(discipline) + " | Average: " + str(average_grade))
                            elif command == "0":
                                break
                            else:
                                raise ValueError(Fore.RED + "Invalid command" + Style.RESET_ALL)
                        except ValueError as ve:
                            print(Fore.RED + f"{ve}" + Style.RESET_ALL)
                elif command == "0":
                    print(Fore.MAGENTA + "Bye!" + Style.RESET_ALL)
                    break
                else:
                    print(Fore.RED + "Invalid command" + Style.RESET_ALL)
        except ValueError as ve:
            print(Fore.RED + f"{ve}" + Style.RESET_ALL)

    @classmethod
    def menu(cls):
        print(Fore.YELLOW + "1. Manage students and disciplines" + Style.RESET_ALL)
        print(Fore.YELLOW + "2. Grade student" + Style.RESET_ALL)
        print(Fore.YELLOW + "3. Search(students/disciplines)" + Style.RESET_ALL)
        print(Fore.YELLOW + "4. Statistics" + Style.RESET_ALL)
        print(Fore.YELLOW + "0. Exit" + Style.RESET_ALL)

    @classmethod
    def menu_manage(cls):
        print(Fore.YELLOW + "1. Add student")
        print("2. Add discipline" + Style.RESET_ALL)
        print(Fore.RED + "3. Remove student")
        print("4. Remove discipline" + Style.RESET_ALL)
        print(Fore.CYAN + "5. Update student")
        print("6. Update discipline" + Style.RESET_ALL)
        print(Fore.GREEN + "7. List students")
        print("8. List disciplines" + Style.RESET_ALL)
        print(Fore.YELLOW + "0. Back" + Style.RESET_ALL)

    @classmethod
    def menu_search(cls):
        print(Fore.YELLOW + "1. Student - ID")
        print("2. Student - name(partial string matching)")
        print("3. Discipline - ID")
        print("4. Discipline - name(partial string matching)")
        print("0. Back" + Style.RESET_ALL)

    @classmethod
    def menu_statistitcs(cls):
        print(Fore.YELLOW + "1. List failing students")
        print("2. Show student situation")
        print("3. Show discipline situation")
        print("0. Back" + Style.RESET_ALL)

if __name__ == "__main__":
    repo = StudentsRegisterRepository()
    service = Service(repo)
    ui = UI(service)
    ui.run()
