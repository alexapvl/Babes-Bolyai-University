# Solve the problem from the first set here
# Problem 2

def is_prime(x: int) -> bool:
    """
    The function determines whether x is a prime number by first checking if it is <= 1,
    then if it is 2(the only even prime number) and lastly, goes through every odd divisor
    of x, to check if it has any proper divisors. If one such divisor is found, then x is
    NOT prime and the function returns False. If the function passes past the while loop, then
    that means that x is prime and returns True.
    :param x: int
    :return: True(x is prime) / False(x is NOT prime)
    """
    if x <= 1:
        return False  # Any number less than or equal to 1 is NOT prime
    if x == 2:
        return True  # "2" is the only even prime number
    if x % 2 == 0:
        return False  # Any number over 2 that is even is NOT prime
    # Making a while loop to check for divisors of x
    i = 3
    while i * i <= x:
        if x % i == 0:
            return False
        i += 2
    return True


def solve():
    """
    The function takes as input the value of n for which it determines if there are
    two numbers p1 and p2 such that p1 and p2 are prime and p1 + p2 = n. There will not
    be a case where p1 and p2 are not found (Goldbach's Rule), for any n >= 4.
    :return: Returns the pair p1 and p2.
    """
    # Input value of n
    n = int(input("Enter a natural number (n >= 4), that is NOT prime: "))
    found: bool = False
    """
    The upper limit is n/2 because we don't want to have two of the same pairs.
    e.g. n = 10 => p1 = 3 and p2 = 7 AND p1 = 7 and p2 = 3
    """
    for p1 in range(2, int(n // 2) + 1):
        p2 = n - p1
        if is_prime(p1) and is_prime(p2):
            # if p1 and p2 are prime, output them; if there are more distinct pairs of p1 and p2, they will be outputted
            print([p1, p2], end=" | ")
            found = True

    if not found:
        print("Invalid input.")


solve()  # Calling the solve function.
