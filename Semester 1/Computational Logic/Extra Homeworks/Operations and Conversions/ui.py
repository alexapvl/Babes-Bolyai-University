from colorama import Fore, Style
from utilities import *
from algorithms import *

def print_menu():
    print(Fore.YELLOW + "1. Convert a number from a base to another base" + Style.RESET_ALL)
    print(Fore.YELLOW + "0. Exit" + Style.RESET_ALL)

def ui():
    print(Fore.CYAN + "Apăvăloaiei Alexandru, group 911" + Style.RESET_ALL)
    while True:
        print_menu()
        try:
            user_input = input(">>> ").strip()
            if user_input == "1":
                sb = int(input("Source base: "))
                if valid_base_p(sb):
                    number = input(f"Number in base {sb}: ")
                    if verify_number_in_base_p(number, sb):
                        while True:
                            db = int(input("Destination base: "))
                            if valid_base_p(db):
                                print(Fore.YELLOW + "1. Rapid conversion (only for converting from bases which are powers of 2)" + Style.RESET_ALL)
                                print(Fore.YELLOW + "2. Conversion with intermediary base 10" + Style.RESET_ALL)
                                print(Fore.YELLOW + "3. Substitution method (only for converting from a smaller base to a larger base)" + Style.RESET_ALL)
                                print(Fore.YELLOW + "4. Successive divisions method (only for converting from a larger base to a smaller base)" + Style.RESET_ALL)
                                print(Fore.YELLOW + "0. Go back" + Style.RESET_ALL)
                                method = input(">>> ")
                                if method == "1":
                                    print(rapid_conversion(number, sb, db))
                                    break
                                elif method == "2":
                                    print(conversion_with_intermediary_base_10(number, sb, db))
                                    break
                                elif method == "3":
                                    print(substitution_method(number, sb, db))
                                    break
                                elif method == "4":
                                    print(successive_divisions_method(number, sb, db))
                                    break
                                elif method == "0":
                                    break
                                else:
                                    raise ValueError(Fore.RED + "Choose a valid method!" + Style.RESET_ALL)
            elif user_input == "0":
                break
            else:
                raise ValueError(Fore.RED + "Invalid option!" + Style.RESET_ALL)
        except ValueError as ve:
            print(ve)
