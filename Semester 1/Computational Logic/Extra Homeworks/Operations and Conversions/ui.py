from colorama import Fore, Style
from utilities import *
from algorithms import *

def print_menu():
    print(Fore.YELLOW + "1. Convert a number from a base to another base" + Style.RESET_ALL)
    print(Fore.YELLOW + "2. Perform an operation between two numbers in the same base" + Style.RESET_ALL)
    print(Fore.YELLOW + "0. Exit" + Style.RESET_ALL)

def print_conversion_menu():
    print(Fore.YELLOW + "Choose a conversion method:" + Style.RESET_ALL)
    print(Fore.YELLOW + "1. Rapid conversion (only for converting from bases which are powers of 2)" + Style.RESET_ALL)
    print(Fore.YELLOW + "2. Conversion with intermediary base 10" + Style.RESET_ALL)
    print(Fore.YELLOW + "3. Substitution method (source base < destination base)" + Style.RESET_ALL)
    print(Fore.YELLOW + "4. Successive divisions method (source base > destination base)" + Style.RESET_ALL)
    print(Fore.YELLOW + "0. Go back to main menu" + Style.RESET_ALL)

def print_operation_menu():
    print(Fore.YELLOW + "Choose an operation:" + Style.RESET_ALL)
    print(Fore.YELLOW + "1. Addition" + Style.RESET_ALL)
    print(Fore.YELLOW + "2. Subtraction" + Style.RESET_ALL)
    print(Fore.YELLOW + "3. Multiplication by one digit" + Style.RESET_ALL)
    print(Fore.YELLOW + "4. Division by one digit" + Style.RESET_ALL)
    print(Fore.YELLOW + "0. Go back to main menu" + Style.RESET_ALL)

def ui():
    print(Fore.CYAN + "Apăvăloaiei Alexandru, group 911" + Style.RESET_ALL)
    while True:
        break_out_flag: bool = False
        print_menu()
        try:
            user_input = input(">>> ").strip()
            if user_input == "1":
                while True:
                    if break_out_flag:
                        break
                    try:
                        sb = input("Source base: ").strip()
                        sb = remove_leading_zeros(sb)
                        if not sb.isdigit():
                            raise ValueError(Fore.RED + "Invalid base!" + Style.RESET_ALL)
                        if valid_base_p(int(sb)):
                            while True:
                                if break_out_flag:
                                    break
                                try:
                                    number = input(f"Number in base {sb}: ").strip().upper()
                                    if verify_number_in_base_p(number, int(sb)):
                                        while True:
                                            if break_out_flag:
                                                break
                                            try:
                                                db = input("Destination base: ").strip()
                                                db = remove_leading_zeros(db)
                                                if not db.isdigit():
                                                    raise ValueError(Fore.RED + "Invalid base!" + Style.RESET_ALL)
                                                if valid_base_p(int(db)):
                                                    while True:
                                                        if break_out_flag:
                                                            break
                                                        try:
                                                            print_conversion_menu()
                                                            method = input("Choose method of conversion: ")
                                                            if method == "1":
                                                                if not valid_base_power_of_2(int(sb)) or not valid_base_power_of_2(int(db)):
                                                                    break_out_flag = True
                                                                    raise ValueError(Fore.RED + f"Current selected bases are {sb} and {db}\nRapid conversion is intended for converting from and to bases which are powers of 2!" + Style.RESET_ALL)
                                                                else:
                                                                    print(f"Number in base {sb}: {number}\nNumber in base {db}: {rapid_conversion(number, int(sb), int(db))}")
                                                                    print(Fore.GREEN + "Converted using rapid conversions" + Style.RESET_ALL)
                                                                    break_out_flag = True
                                                            elif method == "2":
                                                                print(f"Number in base {sb}: {number}\nNumber in base {db}: {conversion_with_intermediary_base_10(number, int(sb), int(db))}")
                                                                print(Fore.GREEN + "Converted using intermediary base 10" + Style.RESET_ALL)
                                                                break_out_flag = True
                                                            elif method == "3":
                                                                if int(sb) <= int(db):
                                                                    print(f"Number in base {sb}: {number}\nNumber in base {db}: {substitution_method(number, int(sb), int(db))}")
                                                                    print(Fore.GREEN + "Converted using substitution method" + Style.RESET_ALL)
                                                                    break_out_flag = True
                                                                else:
                                                                    break_out_flag = True
                                                                    raise ValueError(Fore.RED + "Substitution method is intended for converting from a smaller base to a larger base!" + Style.RESET_ALL)
                                                            elif method == "4":
                                                                if int(sb) >= int(db):
                                                                    print(f"Number in base {sb}: {number}\nNumber in base {db}: {successive_divisions_method(number, int(sb), int(db))}")
                                                                    print(Fore.GREEN + "Converted using successive divisions method" + Style.RESET_ALL)
                                                                    break_out_flag = True
                                                                else:
                                                                    break_out_flag = True
                                                                    raise ValueError(Fore.RED + "Successive divisions method is intended for converting from a larger base to a smaller base!" + Style.RESET_ALL)
                                                            elif method == "0":
                                                                break_out_flag = True
                                                            else:
                                                                raise ValueError(Fore.RED + "Choose a valid method!" + Style.RESET_ALL)
                                                        except ValueError as ve:
                                                            print(ve)
                                            except ValueError as ve:
                                                print(ve)
                                except ValueError as ve:
                                    print(ve)
                    except ValueError as ve:
                        print(ve)
            if user_input == "2":
                while True:
                    if break_out_flag:
                        break
                    try:
                        base = input("Base in which operations will be performed: ").strip()
                        base = remove_leading_zeros(base)
                        if not base.isdigit():
                            raise ValueError(Fore.RED + "Invalid base!" + Style.RESET_ALL)
                        if valid_base_p(int(base)):
                            while True:
                                if break_out_flag:
                                    break
                                try:
                                    number1 = input(f"First number in base {base}: ").strip().upper()
                                    if verify_number_in_base_p(number1, int(base)):
                                        while True:
                                            if break_out_flag:
                                                break
                                            try:
                                                number2 = input(f"Second number in base {base}: ").strip().upper()
                                                if verify_number_in_base_p(number2, int(base)):
                                                    while True:
                                                        if break_out_flag:
                                                            break
                                                        try:
                                                            print_operation_menu()
                                                            operation = input(">>> ").strip()
                                                            if operation == "1":
                                                                print(f"{number1} + {number2} = {addition_in_base_p(number1, number2, int(base))}")
                                                                print(Fore.GREEN + f"Addition performed in base {base}" + Style.RESET_ALL)
                                                            elif operation == "2":
                                                                print(f"{number1} - {number2} = {subtraction_in_base_p(number1, number2, int(base))}")
                                                                print(Fore.GREEN + f"Subtraction performed in base {base}" + Style.RESET_ALL)
                                                            elif operation == "3":
                                                                print(f"{number1} x {number2} = {multiplication_by_one_digit_in_base_p(number1, number2, int(base))}")
                                                                print(Fore.GREEN + f"Multiplication performed in base {base}" + Style.RESET_ALL)
                                                            elif operation == "4":
                                                                quotient, remainder = division_by_one_digit_in_base_p(number1, number2, int(base))
                                                                print(f"{number1} / {number2} = {quotient}, remainder: {remainder}")
                                                                print(Fore.GREEN + f"Division performed in base {base}" + Style.RESET_ALL)
                                                            elif operation == "0":
                                                                break_out_flag = True
                                                            else:
                                                                raise ValueError(Fore.RED + "Choose a valid operation!" + Style.RESET_ALL)
                                                        except ValueError as ve:
                                                            print(ve)
                                            except ValueError as ve:
                                                print(ve)
                                except ValueError as ve:
                                    print(ve)
                    except ValueError as ve:
                        print(ve)
            elif user_input == "0":
                break
            else:
                raise ValueError(Fore.RED + "Invalid option!" + Style.RESET_ALL)
        except ValueError as ve:
            print(ve)
