from colorama import Fore, Style
from utilities import *

def addition_in_base_p(a: str, b: str, p: int) -> str:
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
    if len(a) < len(b):
        # commutativity
        return multiplication_by_one_digit_in_base_p(b, a, p)
    if len(b) != 1:
        raise ValueError(Fore.RED + "Multiplicasion only by one digit! (second param. for multiplication needs to be asingle digit)" + Style.RESET_ALL)
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
    digits = "0123456789ABCDEF"
    if len(b) != 1:
        raise ValueError(Fore.RED + "Division only by one digit! (second param. for division needs to be a single digit)" + Style.RESET_ALL)
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
