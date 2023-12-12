import re
from random import randint, choice
from math import sqrt
from copy import deepcopy
from colorama import Fore, Style

#
# The program's functions are implemented here. There is no user interaction in this file, therefore no input/print statements. Functions here
# communicate via function parameters, the return statement and raising of exceptions. 
#

def expression_validator(input_text: str):
    """Validate a given user command using the "power" of regex.

    Args:
        input_text (str): user command

    Returns:
        _type_: True if command is valid and false otherwise.
    """
    patterns = [re.compile(r"^add\s(0|(-|)[1-9]+\d*)(\+|-)(0i|[1-9]+\d*i)$"),  # add <number>
                re.compile(r"^insert\s(0|(-|)[1-9]+\d*)(\+|-)(0i|[1-9]+\d*i|i)\sat\s([1-9]+\d*|0)$"),  # insert <number> at <position>
                re.compile(r"^remove\s(([1-9]+\d*)|0)$"),  # remove <position>
                re.compile(r"^remove\s([1-9]+\d*|0)\sto\s([1-9]+\d*|0)$"),  # remove <start position> to <end position>
                re.compile(r"^replace\s(0|(-|)[1-9]+\d*)(\+|-)(0i|[1-9]+\d*i|i)\swith\s(0|(-|)[1-9]+\d*)(\+|-)(0i|[1-9]+\d*i|i)$"),  # replace <old number> with <new number>
                re.compile(r"^list$"),  # list
                re.compile(r"^list\sreal\s([1-9]+\d*|0)\sto\s([1-9]+\d*|0)$"),  # list real <start position> to <end position>
                re.compile(r"^list\smodulo\s(<|=|>)\s([1-9]+\d*|0)$"),  # list modulo [ < | = | > ] <number>
                re.compile(r"^filter\sreal$"),  # filter real
                re.compile(r"^filter\smodulo\s(<|=|>)\s([1-9]+\d*|0)$"),  # filter modulo [ < | = | > ] <number>
                re.compile(r"^undo$"), # undo
                re.compile(r"^exit$")] # exit
    for pattern in patterns:
        if re.match(pattern, input_text):
            return True  # Input matches one of the patterns
    raise ValueError(Fore.RED + f"Invalid command -> {input_text}" + Style.RESET_ALL)  # Input does not match any of the patterns

def update_history(data: list, history: list) -> list:
    """Updates and returns the updated history.

    Args:
        data (list): Current list of numbers
        history (list): Current history

    Returns:
        list: Updated history
    """
    history.append(deepcopy(data))
    return history

def add(number: str, data: list) -> list:
    """Appends a number to the list.

    Args:
        number (str): Number to be added
        data (list): List of numbers to which the number should be added

    Returns:
        list: The new list with the appended number
    """
    data.append(number)
    return data

def insert(number: str, position: int, data: list) -> list:
    """Inserts a number at a given position in the list.

    Args:
        number (str): Number to be inserted
        position (int): Index at which the number should be inserted
        data (list): Numbers list

    Returns:
        list: The new list with the inserted number
    """
    data.insert(position, number)
    return data

def get_real(text: str) -> str:
    """Returns, as a string, the real part of a complex number of the form "a+bi" where a and b are intigers.

    Args:
        text (str): Complex number

    Returns:
        str: Real part of the complex number, as a string
    """
    if "+" in text:
        x = text.split("+")
        return x[0]
    else:
        x = text.split("-")
        if text[0] == "-":
            return "-" + x[1] # x[1] because when '-' is the first character, x=['', 'real', 'im']
        else:
            return x[0]

def get_imaginary(text: str) -> str:
    """Returns, as a string, the imaginary part of a complex number of the form "a+bi" where a and b are intigers.

    Args:
        text (str): Complex number

    Returns:
        str: Imaginary part of the complex number, as a string
    """
    if '+' in text:
        x = text.split('+')
        return x[1][:len(x[1])-1]
    else:
        x = text[1:].split('-')
        if "0i" in x:
            return "0"
        elif "1" in x:
            return "-1"
        else:
            return "-" + x[1][:len(x[1])-1]

def format_number(text: str) -> str:
    """Given a complex number of the form "a+bi", it will be formatted in such a way so it is written in natural language.
    For example: "0+0i" => "0"
                 "1+1i" => "1+i"
                 "0+1i" => "i" etc.
    Args:
        text (str): User inputted number

    Returns:
        str: The fomatted number
    """
    if '+' in text:
        operator = "+"
    else:
        operator = "-"
    if get_real(text) == "0" and get_imaginary(text) == "0":
        return "0"
    elif get_real(text) == "0":
        if get_imaginary(text) == "1":
            return "i"
        elif get_imaginary(text) == "-1":
            return "-i"
        else:
            return get_imaginary(text)+"i"
    elif get_imaginary(text) == "0":
        return get_real(text)
    elif get_imaginary(text) == "1" or get_imaginary(text) == "-1":
        return get_real(text)+operator+"i"
    else:
        if int(get_imaginary(text)) < 0:
            return get_real(text)+get_imaginary(text)+"i"
        else:
            return get_real(text)+operator+get_imaginary(text)+"i"
    
def generate_sample() -> list:
    """Generates a sample of 10 random numbers which are not formatted.

    Returns:
        list: The generated list of complex numbers.
    """
    number_list = []
    for i in range(10):
        number: str = str(randint(-20,20)) + choice(["+", "-"]) + str(randint(0,20)) + "i"
        number_list.append(number)
    return number_list

def modulo(text: str) -> float:
    """Calculates the modulo of a complex number.

    Args:
        text (str): The number, not formatted

    Returns:
        float: Modulo of the complex number
    """
    return round(sqrt(int(get_real(text)) ** 2 + int(get_imaginary(text)) ** 2), 4)

def remove(data: list, index_to_remove: int) -> list:
    """Removes a number from the list at a given index.

    Args:
        data (list): Numbers list
        index_to_remove (int): Index of the number to be removed

    Returns:
        list: The new list with the removed number
    """
    data.pop(index_to_remove)
    return data

def remove_left_right(data: list, index_left: int, index_right: int) -> list:
    """Removes all the numbers from the list between two given indexes.

    Args:
        data (list): Numbers list
        index_left (int): Left index
        index_right (int): Right index

    Returns:
        list: The new list with the removed numbers
    """
    data = data[:index_left] + data[index_right + 1:]
    return data

def replace(number_to_replace: str, replacement: str, data: list) -> list:
    """Replaces a number with another number in the list.

    Args:
        number_to_replace (str): Number to be replaced
        replacement (str): Number to replace with
        data (list): Numbers list

    Returns:
        list: The new list with the replaced number
    """
    for i in range(len(data)):
        if data[i] == number_to_replace:
            data[i] = replacement
    return data

def verify_index_remove(data: list, index_left: int, index_right: int) -> bool:
    """Verifies if the indexes are sintactically correct / valid.

    Args:
        data (list): Numbers list
        index_left (int): Left index
        index_right (int): Right index

    Returns:
        bool: True if indexes are valid and false otherwise.
    """
    if index_left <= index_right and index_left >= 0 and index_right >= 0 and index_left <= len(data) - 1 and index_right <= len(data) - 1:
        return data
    else:
        if index_left > index_right:
            raise ValueError(Fore.RED + "Left index cannot be greater than right index!" + Style.RESET_ALL)
        else:
            raise ValueError(Fore.RED + "Indexes out of list range. List not was NOT modified!" + Style.RESET_ALL)
    
def list_modulo(data: list, comparison_operand: str, comparison_number: int):
    """Generates a list of tuples that corresponds with the given parameters and each tuple has the index at which
    the number is found in the list and the number itself.

    Args:
        data (list): List of numbers
        comparison_operand (str): Comparison operand: '<', '>' or '='
        comparison_number (int): Value of modulo to which the number's modulo needs to be compared

    Returns:
        list_of_tuples: Each tuple has the index at which the number is found in the list and the number itself
    """
    result = [] # list of tuples
    if comparison_operand == "<":
        for index, number in enumerate(data):
            if modulo(number) < comparison_number:
                result.append((index, format_number(number)))
    elif comparison_operand == "=":
        for index, number in enumerate(data):
            if modulo(number) == comparison_number:
                result.append((index, format_number(number)))
    elif comparison_operand == ">":
        for index, number in enumerate(data):
            if modulo(number) > comparison_number:
                result.append((index, format_number(number)))
    return result

def list_real(data: list, start: int, end: int) -> list:
    """Generates a list which is made up of only numbers with null imaginary parts between 2 indexes.

    Args:
        data (list): Numbers list
        start (int): Leftmost index
        end (int): Rightmost index

    Returns:
        list: List with only numbers with null imaginary parts that are between the two given indexes
    """
    result = [] # list of tuples
    for index in range(start, end+1):
        if get_imaginary(data[index]) == "0":
            result.append((index, format_number(data[index])))
    return result

def filter_real(data: list) -> list:
    """Generates a list which is only composed of complex numbers with null imaginary part.

    Args:
        data (list): Numbers list

    Returns:
        list: The filtered list
    """
    result = []
    for number in data:
        if get_imaginary(number) == "0":
            result.append(number)
    return result
                 
def filter_modulo(data: list, comparison_operand: str, comparison_number: int) -> list:
    """Generates a list from the numbers list that is only composed of numbers with modulo '<', '>' or '=' to
    a given value.

    Args:
        data (list): List of numbers
        comparison_operand (str): Comparison operand: '<', '>' or '='
        comparison_number (int): Value of modulo to which the number's modulo needs to be compared

    Returns:
        list: The filtered list
    """
    result = []
    if comparison_operand == "<":
        for number in data:
            if modulo(number) < comparison_number:
                result.append(number)
    elif comparison_operand == "=":
        for number in data:
            if modulo(number) == comparison_number:
                result.append(number)
    elif comparison_operand == ">":
        for number in data:
            if modulo(number) > comparison_number:
                result.append(number)
    return result
     