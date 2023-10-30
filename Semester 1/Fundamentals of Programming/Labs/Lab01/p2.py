# Solve the problem from the second set here
# Problem 10

"""
The palindrome of a number is the number obtained by reversing
the order of its digits (e.g. the palindrome of 237 is 732).
For a given natural number n, determine its palindrome.
"""


def find_palindrome(x: int) -> int:
    """
    Computes the palindrome of x.
    :param x: The number for which we need to determine the palindrome.
    :return: The value p, where p is the palindrome of x.
    """
    p = 0
    while x > 0:
        p = p * 10 + x % 10
        x = x // 10  # OBS: "/" is for float division and "//" is for integer division!
    return p


def solve():
    """
    Takes as input the value of n and then computes the palindrome of n and outputs it.
    :return: The palindrome of the given number n.
    """
    n = int(input("Enter the value of n: "))  # Takes the input of value n

    print(f"The palindrome of {n} is {find_palindrome(n)}.")  # Prints the palindrome of n


solve()  # Calling the solve function.

"""
Observation: the letter "f" is used to create an f-string, in order
to embed expressions inside string literals
"""
