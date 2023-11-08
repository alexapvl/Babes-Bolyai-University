# Cocktail Sort and Heap Sort

"""
Cocktail Sort is a more efficient version of Bubble Sort that sorts the elements of the array by
going in both directions (left -> right and right -> left) until the array is sorted.
"""
# We import random in order to be able to generate a list of 'n' random natural numbers
import random


def gen_random_list(n: int) -> list:
    """
    This function generates a random list using the random library.
    :param n: The number of elements the random generated list should have.
    :return: The random generated list.
    """
    if n <= 0:
        return []  # In case the input is not valid.

    random_list = []
    for i in range(0, n):
        #  The random generated number is 'x' using random.randint(start, end) function
        x = random.randint(1, 100)

        random_list.append(x)  # Adds 'x' to the list

    return random_list  # Returns the random list generated


def swap(x: list, i: int, j: int):
    """
    Swap function for more convenience. Will be used in both sorting algorithms.
    :param x: The list
    :param i: The first index.
    :param j: The second index.
    """
    x[i], x[j] = x[j], x[i]


def cocktail_sort(x: list, step: int):
    """
    Cocktail Sort is a variation of Bubble sort. The Bubble sort algorithm always traverses
    elements from left and moves the largest element to its correct position in the first
    iteration and second-largest in the second iteration and so on. Cocktail Sort traverses
    through a given array in both directions alternatively. Cocktail sort does not go through
    the unnecessary iteration making it efficient for large arrays.
    :param x: The list given.
    :param step: The frequency at which the current sorted list should be displayed.
    :return: No returns, only outputs in the console.
    """
    length: int = len(x)
    swapped: bool = True
    iteration: int = 0  # One iteration means one go from left to right OR right to left!
    """
    The "start" and "end" variables are incremented and decremented respectfully because
    after each iteration, the values at the extremities will be it their designated spot.
    """
    start: int = 0
    end: int = length - 1
    while swapped:
        """
        Reset the swapped value on entering the loop because
        it might be true from a previous iteration.
        """
        swapped = False

        # Loop from left to right just like in bubble sort
        for i in range(start, end):
            if x[i] > x[i + 1]:
                swap(x, i, i + 1)  # Swap the values
                swapped = True

        iteration = iteration + 1
        if iteration % step == 0:
            print(f"The list at step {iteration} is: {x}")

        if not swapped:
            break  # The list is sorted if no swaps occurred

        """
        We decrement "end" because by going from left to right, the greatest element
        which is not in its designated spot will be placed accordingly.
        """
        end = end - 1
        swapped = False  # Reset the swapped value upon entering the second loop because

        # Loop from right to left
        for i in range(end, start, -1):
            if x[i] < x[i - 1]:
                swap(x, i, i - 1)
                swapped = True

        iteration = iteration + 1
        if iteration % step == 0:
            print(f"The list at step {iteration} is: {x}")

        """
        We increment "start" because by going from right to left, the smallest element
        which is not in its designated spot will be placed accordingly.
        """
        start = start + 1

    if iteration % step != 0:
        print(f"The sorted list is: {x}")  # Prints the list after sorting it if it was not printed at the last step


def sift_down(x: list, i: int, upper: int):
    """
    Function is used to heapify the list in the heap_sort function.
    By sifting down we imagine that we have a binary tree that is composed of the elements of the list.
    With each iteration, the largest element is put in the root of the binary tree and then swapped with the last
    element of the list (in our case that the root will eventually be 9 and wil be swapped with 7). This process is
    done until we are left with only the root, no additional children and only then, we will know for sure that the
    list will be sorted.
    e.g. list is {1,4,3,2,5,9,7} where 1 is the root and 4 and 3 are the children of one, 2 and 5 the children of 4
    and 9 and 7 the children of 3. This binary tree is formed in the order that the elements are given in the list.
                1
            4      3
        2      5  9     7
    :param x: The list that we need to heapify.
    :param i:
    :param upper: The index at which the heapify stops. We have such an index because after each iteration of sifting
    down we will swap the first element of the list with the element at the index upper - 1 because the first element
    is going to be the greatest out of those that are unsorted at any given moment.
    :return:
    """
    while True:
        l: int = i * 2 + 1  # Left child of element on index "i"
        r: int = i * 2 + 2  # Right child of element on index "i"
        if max(l, r) < upper:  # If the element on index "i" has both children
            if x[i] >= max(x[l], x[r]):
                break
            elif x[l] > x[r]:
                swap(x, i, l)
                i = l  # Move down the parent for our parent node
            else:
                swap(x, i, r)
                i = r  # Move down the parent for our parent node
        elif l < upper:  # If we have only a left child
            if x[l] > x[i]:
                swap(x, i, l)
                i = l  # Move down the parent for our parent node
            else:
                break
        elif r < upper:  # If we have only a right child
            if x[r] > x[i]:
                swap(x, r, i)
                i = r  # Move down the parent for our parent node
            else:
                break
        else:
            break


def heap_sort(x: list, step: int):
    for i in range((len(x) - 2) // 2, -1, -1):
        # (len(x) - 2) // 2 is the last possible index for a parent depending on the length of the list
        sift_down(x, i, len(x))

    for j in range(len(x)-1, 0, -1):  # This will actually do the sorting of the list
        swap(x, 0, j)
        sift_down(x, 0, j)
        if (len(x) - j) % step == 0:
            print(f"The list at step {len(x) - j} is: {x}")

    if (len(x) - j) % step != 0:
        print(f"The sorted list is: {x}")  # Prints the list after sorting it if it was not printed at the last step


def main():
    n = int(input("How many elements should the first generated list have? "))
    random_list: list = gen_random_list(n)  # Generate the random list of n elements
    copy_of_random_list: list = random_list.copy()
    while True:
        print("Generated numbers in the list are between 0 and 100.")
        print("-----------------------------------------")
        print("1. Sort the list using Cocktail Sort.")
        print("2. Sort the list using Heap Sort.")
        print("3. Generate a new random list.")
        print("0. Exit.")
        print("-----------------------------------------")
        option = int(input("Option: "))

        if option == 1:
            # n = int(input("How many elements should the list have? "))
            # random_list = gen_random_list(n)  # Generate the random list of n elements
            step = int(input("The value of step is: "))
            print(f"The current randomly generated list is: {random_list}")
            cocktail_sort(random_list, step)
            random_list = copy_of_random_list.copy()
            # We do this in order to not change the initial list if we want to sort it using a different algorithm
        elif option == 2:
            # n = int(input("How many elements should the list have? "))
            # random_list = gen_random_list(n)  # Generate the random list of n elements
            step = int(input("The value of step is: "))
            print(f"The current randomly generated list is: {random_list}")
            heap_sort(random_list, step)
            random_list = copy_of_random_list.copy()
        elif option == 3:
            n = int(input("How many elements should the newly generated list have? "))
            random_list = gen_random_list(n)
            # Also change the copy of the initial list in case a new one is generated
            copy_of_random_list = random_list.copy()
            print("1. Sort the new list using Cocktail Sort.")
            print("2. Sort the new list using Heap Sort.")
            print("-----------------------------------------")
            new_option = int(input("How would you like to sort the new list? "))
            if new_option == 1:
                step = int(input("The value of step is: "))
                print(f"The randomly generated list is: {random_list}")
                cocktail_sort(random_list, step)
                random_list = copy_of_random_list.copy()
            elif new_option == 2:
                step = int(input("The value of step is: "))
                print(f"The randomly generated list is: {random_list}")
                heap_sort(random_list, step)
                random_list = copy_of_random_list.copy()
            else:
                print("Invalid option. Try again.")
        elif option == 0:
            print("Bye! See you soon!")
            break
        else:
            print("Invalid option. Try again.")


main()
