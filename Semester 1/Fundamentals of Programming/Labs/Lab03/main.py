import random
import time

def gen_random_list(n: int) -> list:
    """
    This function generates a random list using the random library.
    :param n: The number of elements the random generated list should have.
    :return: The random generated list.
    """
    if n <= 0:
        return []
    return random.sample(range(1, n+1), n)


def list_best_case(n: int) -> list:
    """
    This function generates a list of n values that is already sorted in order to get the best case time.
    :param n: The number of elements the random generated list should have.
    :return: The best case list.
    """
    if n <= 0:
        return []
    return list(range(1, n+1))


def list_worst_case(n: int) -> list:
    """
    This function generates a list of n values that is sorted, but in reverse in order to get the worst case time.
    :param n: The number of elements the random generated list should have.
    :return: The worst case list.
    """
    if n <= 0:
        return [] 
    return list(range(n, 0, -1))


def swap(x: list, i: int, j: int):
    """
    Swap function for more convenience. Will be used in both sorting algorithms.
    :param x: The list
    :param i: The first index.
    :param j: The second index.
    """
    x[i], x[j] = x[j], x[i]


def cocktail_sort(x: list):
    # The time complexity of cocktail_sort algorithm is O(n^2) because we have a "while" loop that goes through the list
    # and a "for" loop that goes through the list as well. 
    # We can calculate this complexity like this: n + n-1 + n-2 + ... + 1 = n(n+1)/2 = n^2 + n/2 = O(n^2) because n^2 is
    # the dominant term
    # Best case complexity is O(n) because we have a "while" loop that goes through the list and a "for" loop that goes
    # through the list as well.
    # Worst case complexity is O(n^2) because we have a "while" loop that goes through the list and a "for" loop that
    # goes through the list as well.
    
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
    copy_list = x.copy()
    start_time = time.time()  # Started the timer
    length: int = len(copy_list)
    swapped: bool = True
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
            if copy_list[i] > copy_list[i + 1]:
                swap(copy_list, i, i + 1)  # Swap the values
                swapped = True

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
            if copy_list[i] < copy_list[i - 1]:
                swap(copy_list, i, i - 1)
                swapped = True
        """
        We increment "start" because by going from right to left, the smallest element
        which is not in its designated spot will be placed accordingly.
        """
        start = start + 1

    end_time = time.time()  # End of the timer
    elapsed_time = end_time - start_time

    print(f"Cocktail Sort: {elapsed_time: .4f} seconds")


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


def heap_sort(x: list):

    # The time complexity of the heap_sort algorithm is O(nlogn) because we have a "for" loop that goes through the list
    # We can calculate this complexity like this: n + n/2 + n/4 + n/8 + ... + 1 = n(1 + 1/2 + 1/4 + 1/8 + ... + 1/n) = nlogn
    # Best case complecity is O(nlogn) because we have a "for" loop that goes through the list.
    # Worst case complexity is O(nlogn) because we have a "for" loop that goes through the list.
    # OBS: The height of a complete binary tree having n elements is logn !!!!!

    copy_list = x.copy()
    start_time = time.time()  # Started the timer

    for i in range((len(copy_list) - 2) // 2, -1, -1):
        # (len(x) - 2) // 2 is the last possible index for a parent depending on the length of the list
        sift_down(copy_list, i, len(copy_list))

    for j in range(len(copy_list) - 1, 0, -1):  # This will actually do the sorting of the list
        swap(copy_list, 0, j)
        sift_down(copy_list, 0, j)

    end_time = time.time()  # End of the timer
    elapsed_time = end_time - start_time
    print(f"    Heap Sort: {elapsed_time: .4f} seconds")


def main():
    # We do not make list copies because the functions are defined such that the original list is not modified at all
    while True:
        print("1. Best case sorting time.")
        print("2. Average case sorting time.")
        print("3. Worst case sorting time.")
        print("0. Exit.")
        print("----------------------------")
        option = int(input("Select option: "))

        if option == 1:
            list_1 = list_best_case(500)
            list_2 = list_best_case(1000)
            list_3 = list_best_case(2000)
            list_4 = list_best_case(4000)
            list_5 = list_best_case(8000)
            print("500 elements:")
            cocktail_sort(list_1)
            heap_sort(list_1)
            print("----------------------------")
            print("1000 elements:")
            cocktail_sort(list_2)
            heap_sort(list_2)
            print("----------------------------")
            print("2000 elements:")
            cocktail_sort(list_3)
            heap_sort(list_3)
            print("----------------------------")
            print("4000 elements:")
            cocktail_sort(list_4)
            heap_sort(list_4)
            print("----------------------------")
            print("8000 elements:")
            cocktail_sort(list_5)
            heap_sort(list_5)
            print("----------------------------")
        elif option == 2:
            list_1 = gen_random_list(500)
            list_2 = gen_random_list(1000)
            list_3 = gen_random_list(2000)
            list_4 = gen_random_list(4000)
            list_5 = gen_random_list(8000)
            print("500 elements:")
            cocktail_sort(list_1)
            heap_sort(list_1)
            print("----------------------------")
            print("1000 elements:")
            cocktail_sort(list_2)
            heap_sort(list_2)
            print("----------------------------")
            print("2000 elements:")
            cocktail_sort(list_3)
            heap_sort(list_3)
            print("----------------------------")
            print("4000 elements:")
            cocktail_sort(list_4)
            heap_sort(list_4)
            print("----------------------------")
            print("8000 elements:")
            cocktail_sort(list_5)
            heap_sort(list_5)
            print("----------------------------")

        elif option == 3:
            list_1 = list_worst_case(500)
            list_2 = list_worst_case(1000)
            list_3 = list_worst_case(2000)
            list_4 = list_worst_case(4000)
            list_5 = list_worst_case(8000)
            print("500 elements:")
            cocktail_sort(list_1)
            heap_sort(list_1)
            print("----------------------------")
            print("1000 elements:")
            cocktail_sort(list_2)
            heap_sort(list_2)
            print("----------------------------")
            print("2000 elements:")
            cocktail_sort(list_3)
            heap_sort(list_3)
            print("----------------------------")
            print("4000 elements:")
            cocktail_sort(list_4)
            heap_sort(list_4)
            print("----------------------------")
            print("8000 elements:")
            cocktail_sort(list_5)
            heap_sort(list_5)
            print("----------------------------")

        elif option == 0:
            print("Bye! See you soon!")
            break
        else:
            print("Invalid value for 'option'.")
            break


main()
