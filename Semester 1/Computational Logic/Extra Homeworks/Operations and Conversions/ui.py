from colorama import Fore, Style
from utilities import *
from algorithms import *

def print_menu():
    print(Fore.YELLOW + "1. Convert a number from a base to another base" + Style.RESET_ALL)
    print(Fore.YELLOW + "0. Exit" + Style.RESET_ALL)

def print_conversion_menu():
    print(Fore.YELLOW + "Choose a conversion method:" + Style.RESET_ALL)
    print(Fore.YELLOW + "1. Rapid conversion (only for converting from bases which are powers of 2)" + Style.RESET_ALL)
    print(Fore.YELLOW + "2. Conversion with intermediary base 10" + Style.RESET_ALL)
    print(Fore.YELLOW + "3. Substitution method (source base < destination base)" + Style.RESET_ALL)
    print(Fore.YELLOW + "4. Successive divisions method (source base > destination base)" + Style.RESET_ALL)
    print(Fore.YELLOW + "0. Go back" + Style.RESET_ALL)

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
                                                        try:
                                                            print_conversion_menu()
                                                            method = input(">>> ")
                                                            if method == "1":
                                                                if not valid_base_power_of_2(int(sb)) or not valid_base_power_of_2(int(db)):
                                                                    print(Fore.RED + f"Current selected bases are {sb} and {db}.\nRapid conversion is intended for converting from and to bases which are powers of 2!" + Style.RESET_ALL)
                                                                    break_out_flag = True
                                                                    break
                                                                else:
                                                                    print(rapid_conversion(number, int(sb), int(db)))
                                                                    break_out_flag = True
                                                                    break
                                                            elif method == "2":
                                                                print(conversion_with_intermediary_base_10(number, int(sb), int(db)))
                                                                break
                                                            elif method == "3":
                                                                if int(sb) <= int(db):
                                                                    print(substitution_method(number, int(sb), int(db)))
                                                                    break
                                                                else:
                                                                    raise ValueError(Fore.RED + "Substitution method is intended for converting from a smaller base to a larger base!" + Style.RESET_ALL)
                                                            elif method == "4":
                                                                if int(sb) >= int(db):
                                                                    print(successive_divisions_method(number, int(sb), int(db)))
                                                                    break
                                                                else:
                                                                    raise ValueError(Fore.RED + "Successive divisions method is intended for converting from a larger base to a smaller base!" + Style.RESET_ALL)
                                                            elif method == "0":
                                                                break
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
            elif user_input == "0":
                break
            else:
                raise ValueError(Fore.RED + "Invalid option!" + Style.RESET_ALL)
        except ValueError as ve:
            print(ve)
