# Author: Apăvăloaiei Alexandru, group 911
from colorama import Fore, Style
from utilities import *

def addition_in_base_p(a: str, b: str, p: int) -> str:
    """
    Performs addition in base p of two numbers a and b.
    First, the numbers are made of the same length by adding leading zeros to the shorter number.
    Then, the addition is performed digit by digit, from right to left.
    If the sum of two digits is greater than or equal to p, then the carry is 1 and the digit is the sum minus the base.
    Otherwise, the carry is 0 and the digit is the sum.
    If the carry is 1 after the last digit, then the carry is added to the result.

    @param a: first number
    @param b: second number
    @param p: base in which the addition is performed
    @return: the result of the addition in base p of a and b
    """
    a, b = make_same_length(a, b)
    result = ""
    carry: int = 0
    for i in range(len(a) - 1, -1, -1):
        digit: int = char_to_digit(a[i]) + char_to_digit(b[i]) + carry
        if digit >= p:
            carry = 1
            digit -= p
        else:
            carry = 0
        result = digit_to_char(digit) + result
    if carry == 1:
        result = "1" + result
    return result

def subtraction_in_base_p(a: str, b: str, p: int) -> str:
    """
    Performs subtraction in base p of two numbers a and b.
    First, the numbers are made of the same length by adding leading zeros to the shorter number.
    Then, the subtraction is performed digit by digit, from right to left.
    If the difference of two digits is negative, then the borrow is 1 and the digit is the difference plus the base.
    Otherwise, the borrow is 0 and the digit is the difference.
    At the end, the leading zeros are removed.

    @param a: first number
    @param b: second number
    @param p: base in which the subtraction is performed
    @return: the result of the subtraction in base p of a and b
    """
    a, b = make_same_length(a, b)
    if base_p_to_base_10(a, p) < base_p_to_base_10(b, p):
        raise ValueError(Fore.RED + "First number must be greater than or equal to the second number!" + Style.RESET_ALL)
    result = ""
    borrow: int = 0
    for i in range(len(a) - 1, -1, -1):
        digit: int = char_to_digit(a[i]) - char_to_digit(b[i]) - borrow
        if digit < 0:
            borrow = 1
            digit += p
        else:
            borrow = 0
        result = digit_to_char(digit) + result
    result = remove_leading_zeros(result)
    return result

def multiplication_by_one_digit_in_base_p(a: str, b: str, p: int) -> str:
    """
    Performs multiplication in base p of a number a and a digit b.
    First, we check if the length of a is smaller than the length of b. If so, there is a chance that a is a digit and b is the number.
    Then, we check if the second parameter is a digit. If not, we raise an error.
    After that, we perform the multiplication digit by digit, from right to left.
    The operations are first performed in base 10, then the result is converted in base p.
    The multiplication is performed from right to left.
    The carry is the result of the division of the product of the digit of a and b by p.
    The digit is the remainder of the division of the product of the digit of a and b by p.
    At the end, if the carry is different from 0, then it is added to the result and the leading zeros are removed.

    @param a: number to be multiplied
    @param b: the digit by which the multiplication is performed
    @param p: base in which the multiplication is performed
    @return: the result of the multiplication in base p of a and digit b
    """
    if len(a) < len(b):
        # commutativity
        return multiplication_by_one_digit_in_base_p(b, a, p)
    if len(b) != 1:
        raise ValueError(Fore.RED + "Multiplicasion only by one digit!" + Style.RESET_ALL)
    result = ""
    carry: int = 0
    for i in range(len(a) - 1, -1, -1):
        digit: int = (base_p_to_base_10(a[i], p) * base_p_to_base_10(b, p) + carry) % p
        carry = (base_p_to_base_10(a[i], p) * base_p_to_base_10(b, p) + carry) // p
        result = digit_to_char(digit) + result
    if carry != 0:
        result = digit_to_char(carry) + result
    result = remove_leading_zeros(result)
    return result

def division_by_one_digit_in_base_p(a: str, b: str, p: int) -> tuple:
    """
    Performs division in base p of a number a and a digit b.
    First, we check if the second parameter is a digit. If not, we raise an error.
    Then, we check if the second parameter is 0. If so, we raise an error because we cannot divide by 0.
    After that, we perform the algorithm for division like we would on paper.
    All operations are performed in base 10, then the result is converted in base p.
    The division is performed from left to right.

    @param a: divident
    @param b: divisor (digit)
    @param p: base in which the division is performed
    @return: a tuple containing the quotient and the remainder of the division in base p of a and digit b
    """
    digits = "0123456789ABCDEF"
    if len(b) != 1:
        raise ValueError(Fore.RED + "Division only by one digit!" + Style.RESET_ALL)
    if b == "0":
        raise ValueError(Fore.RED + "ERROR: Division by zero!" + Style.RESET_ALL)
    quotient = ""
    divisor: int = base_p_to_base_10(b, p) # divisor is converted in base 10, like in the algorithm
    remainder: str = ""
    for i in range(len(a)):
        remainder = remainder + a[i]
        quotient = quotient + digits[base_p_to_base_10(remainder, p) // divisor]
        remainder = digits[base_p_to_base_10(remainder, p) % divisor]
    quotient = remove_leading_zeros(quotient)
    return quotient, remainder
