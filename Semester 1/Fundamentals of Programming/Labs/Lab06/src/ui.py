from functions import *
from colorama import Fore, Style
from copy import deepcopy

#
# This is the program's UI module. The user interface and all interaction with the user (print and input statements) are found here
#

def ui():
    numbers_list = generate_sample() # list of 10 random complex numbers
    history = [] # this will be a list of tuples consisting of last_command and the current numbers_list
    history.append(deepcopy(numbers_list))
    print(Fore.LIGHTMAGENTA_EX + "Welcome to the program" + Style.RESET_ALL)
    while True:
        try:
            user_input = input(">>")
            if expression_validator(user_input):
                command = user_input.split()
                if(command[0] == "add"):
                    history = update_history(numbers_list, history)
                    numbers_list = add(command[1], numbers_list)
                    print(Fore.GREEN + "Added " + Style.RESET_ALL + f"{format_number(command[1])} " + Fore.GREEN + "to the list." + Style.RESET_ALL)
                elif(command[0] == "insert"):
                    if int(command[3]) <= len(numbers_list) - 1:
                        history = update_history(numbers_list, history)
                        numbers_list = insert(command[1], int(command[3]), numbers_list)
                        print(Fore.GREEN + "Inserted " + Style.RESET_ALL + f"{format_number(command[1])} " + Fore.GREEN + "at index " + Style.RESET_ALL + f"{command[3]}")
                    else:
                        raise ValueError(Fore.RED + "Index out of list range. List not was NOT modified!" + Style.RESET_ALL)
                elif(command[0] == "remove"):
                    if len(command) == 2:
                        index_to_remove = int(command[1])
                        if index_to_remove <= len(numbers_list) - 1:
                            history = update_history(numbers_list, history)
                            print(Fore.GREEN + "Successfully removed number at index " + Style.RESET_ALL + f"{index_to_remove}")
                            numbers_list = remove(numbers_list, index_to_remove)
                        else:
                            raise ValueError(Fore.RED + "Index out of list range. List not was NOT modified!" + Style.RESET_ALL)
                    else: # len(command) will be 4
                        index_left = int(command[1])
                        index_right = int(command[3])
                        if verify_index_remove(numbers_list, index_left, index_right):
                            history = update_history(numbers_list, history)
                            numbers_list = remove_left_right(numbers_list, index_left, index_right)
                            print(Fore.GREEN + "Successfully removed numbers from index " + Style.RESET_ALL + f"{index_left} " + Fore.GREEN + "to index " + Style.RESET_ALL + f"{index_right}" )
                elif(command[0] == "replace"):
                    number_to_replace = command[1]
                    replacement = command[3]
                    if command[1] in numbers_list: # We know there will be changes so we update history
                        history = update_history(numbers_list, history)
                        numbers_list = replace(number_to_replace, replacement, numbers_list)
                        print(Fore.GREEN + "Replacement successful" + Style.RESET_ALL)
                    else:
                        print(Fore.YELLOW + "No replacements were made since" + Style.RESET_ALL + f"{number_to_replace}" + Fore.YELLOW + "was not found in the list.")
                elif(command[0] == "list"):
                    if len(command) == 1:
                        if len(numbers_list) == 0:
                            print(Fore.YELLOW + "List is empty." + Style.RESET_ALL)
                        else:
                            for i, number in enumerate(numbers_list):
                                print(Fore.CYAN + str(i) + " -> " + Style.RESET_ALL + format_number(number))
                    elif command[1] == "modulo":
                        if int(command[3]) < 0:
                            raise ValueError(Fore.RED + "Modulus of a complex number cannot be negative!" + Style.RESET_ALL)
                        else:
                            modulo_list = list_modulo(numbers_list, command[2], int(command[3])) # Already formatted list
                            if len(modulo_list) == 0:
                                print(Fore.YELLOW + f"There are no elements with modulo {command[2]} {command[3]}" + Style.RESET_ALL)
                            else:
                                print(Fore.YELLOW + f"Numbers with modulo {command[2]} {command[3]} are:" + Style.RESET_ALL)
                                for element in modulo_list:
                                    index, number = element # Separate the tuple in order to be able to print it
                                    print(Fore.CYAN + f"{index} -> " + Style.RESET_ALL + f"{number}")
                    elif command[1] == "real":
                        if int(command[2]) > len(numbers_list) - 1 or int(command[4]) > len(numbers_list) - 1 or int(command[2]) * int(command[4]) < 0:
                            raise ValueError(Fore.RED + "Invalid indexes given!" + Style.RESET_ALL)
                        else:
                            real_list = list_real(numbers_list, int(command[2]), int(command[4]))
                            if len(real_list) != 0:
                                for element in real_list:
                                    index, number = element # Separate the tuple in order to be able to print it
                                    print(Fore.CYAN + f"{index} -> " + Style.RESET_ALL + f"{number}")
                            else:
                                print(Fore.YELLOW + "No numbers were found in the given interval." + Style.RESET_ALL)
                elif(command[0] == "filter"):
                    if command[1] == "real":
                        if len(numbers_list) > len(filter_real(numbers_list)):
                            history = update_history(numbers_list, history)
                            numbers_list = filter_real(numbers_list)
                            print(Fore.GREEN + "List filtered with success." + Style.RESET_ALL)
                        else:
                            print(Fore.YELLOW + "No numbers were filtered. List was NOT changed. History not updated." + Style.RESET_ALL)
                    elif command[1] == "modulo":
                        filter_modulo_list = filter_modulo(numbers_list, command[2], int(command[3]))
                        if len(numbers_list) > len(filter_modulo_list):
                            history = update_history(numbers_list, history)
                            numbers_list = filter_modulo_list
                            print(Fore.GREEN + "List filtered with success." + Style.RESET_ALL)
                        else:
                            print(Fore.YELLOW + "No numbers were filtered. List was NOT changed. History not updated." + Style.RESET_ALL)
                elif(command[0] == "undo"):
                    if len(history) <= 1:
                        raise ValueError(Fore.RED + "Cannot undo anymore, already at first state of list." + Style.RESET_ALL)
                    else:
                        numbers_list = history.pop()
                        print(Fore.GREEN + "Undo was successful." + Style.RESET_ALL)
                elif(command[0] == "exit"):
                    print(Fore.LIGHTMAGENTA_EX + "Bye!" + Style.RESET_ALL)
                    break
        except ValueError as ve:
            print(ve)
