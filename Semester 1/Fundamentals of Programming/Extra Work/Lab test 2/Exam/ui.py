from domain import Student
from services import Service

class UI:
    def __init__(self, service: Service):
        self.__service = service

    @staticmethod
    def menu():
        print()
        print("1. Add student")
        print("2. List students")
        print("3. Give bonuses")
        print("4. Display students by given string")
        print("0. Exit")
        print()

    def start(self):
        while True:
            self.menu()
            try:
                command = input(">>> ")
                if command == "0":
                    break
                elif command == "1":
                    while True:
                        try:
                            id = int(input("Enter id: "))
                            name = input("Enter name: ")
                            attendance_count = int(input("Enter attendance count: "))
                            grade = int(input("Enter grade: "))
                            self.__service.add(Student(id, name, attendance_count, grade))
                            break
                        except Exception as e:
                            print(str(e))
                elif command == "2":
                    for student in self.__service.get_students_decreasing_order_based_on_grade():
                        print(student.print_string())
                elif command == "3":
                    while True:
                        try:
                            p = int(input("Enter required number of attendances: "))
                            if p < 0:
                                raise Exception("Number of attendances must be a positive integer")
                            b = int(input("Enter number of bonus points: "))
                            if b < 0:
                                raise Exception("Number of bonus points must be a positive integer")
                            self.__service.give_bonuses(p, b)
                            break
                        except Exception as e:
                            print(str(e))
                elif command == "4":
                    string = input("Enter string: ")
                    for student in self.__service.display_students_by_given_string(string):
                        print(student.print_string())
                else:
                    print("Invalid command")
            except ValueError as ve:
                print(str(ve))
            except Exception as e:
                print(str(e))
