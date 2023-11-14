from random import randint
# Problems 4 and 10 - Assigned at lab by prof.
# Write the implementation for A5 in this file
#

#
# Write below this comment
# Functions to deal with complex numbers -- list / dict representation
# -> There should be no print or input statements in this section
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#

def create_complex_number(x: int, y: int, gloal_option: str):
    """
    Creates a complex number

    :param: x: real part
    :param: y: imaginary part
    :param: option: 1 - list representation, 2 - dict representation

    :return: complex number as a list or dict
    """
    if gloal_option == "l" or gloal_option == "list":
        return [x, y]
    elif gloal_option == "d" or gloal_option == "dict":
        return {"real": x, "imaginary": y}
    else:
        raise ValueError("Invalid option")

def generate_random_complex_numbers(n: int, global_option: str) -> list:
    """
    Generates 'n' random complex numbers

    :param: n: number of complex numbers to generate
    :param: global_option: "l" or "list" for list representation, "d" or "dict" for dict representation

    :return: list of 'n' random complex numbers
    """
    data=[]
    while n > 0:
        data.append(create_complex_number(randint(0,100), randint(0, 100), global_option))
        n -= 1
    return data

def to_str(number, global_option: str) -> str:
    """
    Converts a complex number to a string

    :param: number: complex number
    :param: global_option: "l" or "list" for list representation, "d" or "dict" for dict representation

    :return: string representation of the complex number

    OBS: We check whether the real part is less than 10, so that the output is aligned
    """
    if global_option == "l" or global_option == "list" or global_option == "d" or global_option == "dict":
        if get_real(number, global_option) < 10: # For indentation purposes, assuming real part is less than 100
            return "Real part: " + str(get_real(number, global_option)) + "  | Imaginary part: " + str(get_imaginary(number, global_option))
        else:
            return "Real part: " + str(get_real(number, global_option)) + " | Imaginary part: " + str(get_imaginary(number, global_option))
    else:
        raise ValueError("Invalid option")

 # Functions used for problem 4

def number_of_distinct_values(data: list) -> int:
     """
     Finds the number of distinct values in a list

     :param: data: list of numbers

     :return: number of distinct values
     """
     distinct_values = 0
     for i in range(len(data)):
         if data[i] not in data[:i]:
             distinct_values += 1
     return distinct_values

 # Functions used for problem 10

def get_real(number, global_option: str) -> int:
    """
    Returns the real part of a complex number

    :param: number: complex number - can be either in the form of a list or a dictionary
    :param: global_option: "l" or "list" for list representation, "d" or "dict" for dict representation

    :return: real part of the complex number depending on the representation of the complex number
    """
    if global_option == "l" or global_option == "list":
        return number[0]
    elif global_option == "d" or global_option == "dict":
        return number["real"]
    else:
        raise ValueError("Invalid option")

def get_imaginary(number, global_option: str) -> int:
    """

    Returns the imaginary part of a complex number

    :param: number: complex number - can be either in the form of a list or a dictionary
    :param: global_option: "l" or "list" for list representation, "d" or "dict" for dict representation

    :return: imaginary part of the complex number depending on the representation of the complex number
    """
    if global_option == "l" or global_option == "list":
        return number[1]
    elif global_option == "d" or global_option == "dict":
        return number["imaginary"]
    else:
        raise ValueError("Invalid option")

def index_of_largest_element_smaller_than_current_element_real_part(data: list, current_index: int, global_option: str) -> int:
    """
    Determines the index of the largest element smaller than the current element in the list, by comparing the real parts

    :param: data: list of numbers
    :param: current_index: index of the current element

    :return: index of the largest element smaller than the current element, comparing by real parts
    """
    index: int = -1
    for i in range(current_index):
        if get_real(data[i], global_option) < get_real(data[current_index], global_option) and (index == -1 or get_real(data[i], global_option) >= get_real(data[index], global_option)):
            index = i
    return index

#
# Write below this comment
# Functions that deal with subarray/subsequence properties
# -> There should be no print or input statements in this section
# -> Each function should do one thing only
# -> Functions communicate using input parameters and their return values
#

# Problem 4: Length and elements of a longest subarray of numbers that contain at most 3 distinct values. - naive implementation

def longest_subarray_with_3_distinct_values(data: list, global_option: str) -> tuple:
    """
    Finds the longest subarray of numbers that contain at most 3 distinct values by brute force

    The time comlexity of this algorithm is O(n^3) because we have 3 nested loops the 3rd one coming from
    the number_of_distinct_values function which has a time complexity of O(n)

    Space complexity is O(n) because we have a list of numbers as input

    :param: data: list of numbers
    :param: global_option: "l" or "list" for list representation, "d" or "dict" for dict representation

    :return: tuple containing the length of the longest subarray and the subarray itself
    """
    max_length: int = 0
    result: list = []
    if global_option != "l" and global_option != "list" and global_option != "d" and global_option != "dict":
        raise ValueError("Invalid option")
    # Solution for list and dict representation
    for i in range(len(data)):
        for j in range(i, len(data)):
            distinct_values = number_of_distinct_values(data[i:j+1]) # Another O(n) here inside the 2 for loops
            if distinct_values <= 3 and len(data[i:j+1]) > max_length:
                max_length = len(data[i:j+1])
                result = data[i:j+1]

    return max_length, result

# Problem 10 - The length and elements of a longest increasing subsequence, when considering each number's real part

def longest_increasing_subsequence(data: list, global_option: str) -> tuple:
    """
    First, we generate the 'counter' list, which stores the length of the longest increasing subsequence up to index 'i' in the 'data' list
    Then, we generate the 'result' list, which stores the longest increasing subsequence by taking into account the 'counter' list, in reverse order

    The time complexity of this algorithm is O(n^2), where 'n' is the length of the 'data' list
    The first 'n' comes from the for loop, which has a time complexity of O(n)
    The second 'n' comes from the function 'index_of_largest_element_smaller_than_current_element_real_part',
    which has a time complexity of O(n)

    Space complexity: O(n) - we use 2 lists of length 'n'

    :param: data: list of numbers
    :param: global_option: "l" or "list" for list representation, "d" or "dict" for dict representation

    :return: tuple containing the length of the longest increasing subsequence and the subsequence itself
    """
    counter: list = [0] * len(data) # List for counting the longest subsequences up to index 'i' in the 'data' list
    min = float('inf') # Value for the minimun element in the list at index 'i'
    max_length: int = 0 # Length of the longest increasing subsequence
    result: list = [] # List for storing the longest increasing subsequence
    if global_option != "l" and global_option != "list" and global_option != "d" and global_option != "dict":
        raise ValueError("Invalid option")
    # Solution for list and dict representation
    # Form the counter list
    for i in range(len(data)):
        if get_real(data[i], global_option) < min: # If this happens, that means that at index 'i' a new subsequence of length 1 will be formed
            min = get_real(data[i], global_option) # Change the minimum value
            counter[i] = 1 # The length of the longest increasing subsequence at index 'i' is 1
        else:
            # The length of the longest increasing subsequence at index 'i' is 1 + the length of the longest
            # increasing subsequence at the index of the largest element smaller than the current element
            counter[i] = counter[index_of_largest_element_smaller_than_current_element_real_part(data, i, global_option)] + 1
            if counter[i] > max_length:
                max_length = counter[i]
    # We generate the result list by taking into account the counter and data list, in reverse order
    # First, we find the index of the last element in the longest increasing subsequence
    index: int = len(data) - 1
    while counter[index] != max_length:
        index -= 1
    current_max: int = get_real(data[index], global_option) # The current maximum real part that has the longest increasing subsequence
    max_length -= 1
    result.insert(0, data[index])
    while max_length > 0:
        if counter[index] == max_length and get_real(data[index], global_option) < current_max:
            # Because we go from right to left, at each newly found element, we need to put it at the beginning of the result list, as it will
            # be the smallest element in the result list
            result.insert(0, data[index])
            max_length -= 1
        index -= 1

    return len(result), result

#
# Write below this comment
# UI section
# Write all functions that have input or print statements here
# Ideally, this section should not contain any calculations relevant to program functionalities
#

def read_complex_number(data: list, global_option: str):
    """
    Reads a complex number from the user and adds it to the list

    :param: data: list of complex numbers
    :param: global_option: "l" or "list" for list representation, "d" or "dict" for dict representation

    :return: None
    """
    while True:
        try:
            real_part = int(input("Real part: "))
            imaginary_part = int(input("Imaginary part: "))
            if global_option == "l" or global_option == "list" or global_option == "d" or global_option == "dict":
                data.append(create_complex_number(real_part, imaginary_part, global_option))
            else:
                raise ValueError("Invalid option")
            break  # If input and addition to the list are successful, exit the loop
        except ValueError:
            print("Invalid input. Please enter valid integer values for real and imaginary parts.")

def print_list_of_complex_numbers(data: list, global_option: str):
    """
    Prints the list of complex numbers

    :param: data: list of complex numbers
    :param: global_option: "l" or "list" for list representation, "d" or "dict" for dict representation

    :return: None, function only prints the list of complex numbers
    """
    if len(data) == 0:
        raise ValueError("List is empty")
    else:
        if global_option == "l" or global_option == "list":
            for number in data:
                print(to_str(number, global_option))
        elif global_option == "d" or global_option == "dict":
            for number in data:
                print(to_str(number, global_option))

def print_menu():
    """
    Print the menu
    """
    print("--------------------------------------------------")
    print("1. Generate random complex numbers")
    print("2. Add complex number")
    print("3. Print list of complex numbers")
    print("4. Exit")
    print("A. Find longest subarray with 3 distinct values")
    print("B. Find longest increasing subsequence")
    print("--------------------------------------------------")

def start():
    while True:
        global_option = input("Choose representation: l / list, d / dict: ")
        try:
            if global_option != "l" and global_option != "list" and global_option != "d" and global_option != "dict":
                raise ValueError("Invalid option. Try again")
            data = generate_random_complex_numbers(10, global_option)
            while True:
                print_menu()
                option = input(">>> ").strip()
                try:
                    if option == "1":
                        while True:
                            n = input("How many complex numbers should the newly generated list have?\n>>> ")
                            try:
                                if not n.isnumeric():
                                    raise ValueError("Please enter an intiger number")
                                data = generate_random_complex_numbers(int(n), global_option)
                                print("Generated a new list of complex numbers:")
                                break # if input was valid, the loop will be exited
                            except ValueError as ve:
                                print(ve)
                    elif option == "2":
                        while True:
                            how_many = input("How many complex numbers do you want to add?\n>>> ")
                            try:
                                if not how_many.isnumeric():
                                    raise ValueError("Pleas enter an intiger number")
                                for i in range(int(how_many)):
                                    read_complex_number(data, global_option)
                                break # if input was valid, the loop will be exited
                            except ValueError as ve:
                                print(ve)
                    elif option == "3":
                        print_list_of_complex_numbers(data, global_option)
                    elif option == "A" or option == "a":
                        max_lenght, result = longest_subarray_with_3_distinct_values(data, global_option)
                        print(f"The maximum length of the longest subarray with at most 3 distinct values is: {max_lenght}")
                        print("The subarray is:")
                        for element in result:
                            print(to_str(element, global_option))
                    elif option == "B" or option == "b":
                        max_lenght, result = longest_increasing_subsequence(data, global_option)
                        print(f"The maximum length of the longest increasing subsequence is: {max_lenght}")
                        print("The subsequence is:")
                        for element in result:
                            print(to_str(element, global_option))
                    elif option == "4":
                        break
                    else:
                        print("Invalid option")
                except ValueError as ve:
                    print(ve)
            break
        except ValueError as ve:
            print(ve)

if __name__ == "__main__":
    start()
