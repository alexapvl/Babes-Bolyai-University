# Solve the problem from the third set here
# Problem 15

"""
Determines whether a natural number is perfect or not by checking if
the sum of all of its divisors excluding itself is equal to the initial
number itself
"""


def is_perfect(x: int) -> bool:
    """
    Determines whether x is a perfect number or not.
    :param x: int
    :return: True/False
    """
    s: int = 0
    for i in range(1, int(x // 2)):
        if x % i == 0:
            s += i
    if s == x:
        return True
    else:
        return False


# Solution:
def solve():
    """
    The solution takes the value of n after which enters a for loop from n-1 till 1 and searches for
    the first element which is a perfect number.
    :return: Returns the number if found, and if not found, returns a string.
    """
    n = int(input("Enter the value of n: "))
    for i in range(n - 1, 1, -1):
        if is_perfect(i):
            print(f"The largest perfect number smaller than {n} is {i}.")
            found: bool = True
            break

    if not found:
        print("There are no perfect numbers smaller than n.")


solve()  # Calling the solve function.
