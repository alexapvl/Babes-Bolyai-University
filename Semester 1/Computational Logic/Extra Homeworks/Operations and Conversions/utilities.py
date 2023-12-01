from colorama import Fore, Style
from operations import *

def verify_number_in_base_p(a: str, p: int):
    if (p < 2 or p > 10) and p != 16:
        raise ValueError(Fore.RED + "p must be between 2,3,...,10 or 16!" + Style.RESET_ALL)
    digits = "0123456789ABCDEF"
    for i in range(len(a)):
        if a[i] not in digits:
            if a[i] == "-" and i == 0:
                # in case a[i] is negative
                raise ValueError(Fore.RED + "Only positive numbers are allowed!" + Style.RESET_ALL)
            # in case a[i] is not in digits
            raise ValueError(Fore.RED + f"Character {a[i]} in number {a} is not valid!" + Style.RESET_ALL)
        if digits.index(a[i]) >= p:
            # in case a[i] is not a digit in base p
            raise ValueError(Fore.RED + f"Number {a} is not in base {p}!" + Style.RESET_ALL)
    return True

def make_same_length(a: str, b: str) -> tuple:
   a = a.zfill(max(len(a), len(b)))
   b = b.zfill(max(len(a), len(b)))
   return a, b

def remove_leading_zeros(a: str) -> str:
    for i in range(len(a)):
        if a[i] != "0":
            return a[i:]
    return "0"

def digit_to_char(digit: int) -> str:
    digits = "0123456789ABCDEF"
    return digits[digit]

def char_to_digit(a: str) -> int:
    digits = "0123456789ABCDEF"
    return digits.index(a)

def base_p_to_base_10(number: str, p: int) -> int:
    result: int = 0
    for i in range(len(number)):
        result += char_to_digit(number[i]) * p ** (len(number) - i - 1)
    return result

def base_10_to_base_p(number: int, p: int) -> str:
    result = ""
    while number != 0:
        result = digit_to_char(number % p) + result
        number //= p
    return result

def base_p_to_base_2(a: str, p: int):
    result = ""
    power2 = 1
    num_digits = 0
    try:
        if p != 2 and p != 4 and p != 8 and p != 16:
            raise ValueError(Fore.RED + "p must be 2, 4, 8 or 16!" + Style.RESET_ALL)
        if p == 2:
            return a
        while power2 < p:
            power2 *= 2
            num_digits += 1
        for i in range(len(a)):
            result += bin(char_to_digit(a[i]))[2:].zfill(num_digits)
    except ValueError as ve:
        print(ve)
    return remove_leading_zeros(result)

def base_2_to_base_p(a: str, p: int):
    result = ""
    power2 = 1
    num_digits = 0
    try:
        if p != 2 and p != 4 and p != 8 and p != 16:
            raise ValueError(Fore.RED + "p must be 2, 4, 8 or 16!" + Style.RESET_ALL)
        if p == 2:
            return a
        while power2 < p:
            power2 *= 2
            num_digits += 1
        if len(a) % num_digits != 0:
            a = a.zfill(len(a) + (num_digits - len(a) % num_digits))
        for i in range(0, len(a), num_digits):
            result += digit_to_char(int(a[i:i+num_digits], 2))
    except ValueError as ve:
        print(ve)
    return result
