# Author: Apăvăloaiei Alexandru, group 911
from colorama import Fore, Style

def make_same_length(a: str, b: str) -> tuple:
    """
    Makes two numbers of the same length by adding leading zeros to the shorter number.
    OBS: We use zfill() to add leading zeros to a string.
    @param a: first number
    @param b: second number
    @return: a tuple containing the two numbers of the same length
    """
    a = a.zfill(max(len(a), len(b)))
    b = b.zfill(max(len(a), len(b)))
    return a, b

def verify_number_in_base_p(a: str, p: int):
    """
    Verifies if the number a is in base p.

    @param a: number to be verified
    @param p: base in which the number is verified
    @return: True if the number is in base p, False otherwise
    """
    if (p < 2 or p > 10) and p != 16:
        raise ValueError(Fore.RED + "Base must be between 2,3,...,10 or 16!" + Style.RESET_ALL)
    digits = "0123456789ABCDEF"
    if len(a) == 0:
        raise ValueError(Fore.RED + "Number cannot be empty!" + Style.RESET_ALL)
    for i in range(len(a)):
        if a[i] not in digits:
            if a[i] == "-" and i == 0:
                # in case a[i] is negative
                raise ValueError(Fore.RED + "Only positive numbers are allowed!" + Style.RESET_ALL)
            # in case a[i] is not in digits
            raise ValueError(Fore.RED + f"Number {a} is not in base {p}!" + Style.RESET_ALL)
        if digits.index(a[i]) >= p:
            # in case a[i] is not a digit in base p
            raise ValueError(Fore.RED + f"Number {a} is not in base {p}!" + Style.RESET_ALL)
    return True

def valid_base_p(p: int) -> bool:
    """
    Verifies if the base p is valid.
    For our problem statement, a base is valid if it is between 2 and 10 or if it is 16.

    @param p: base to be verified
    @return: True if the base is valid, error raised otherwise
    """
    if (p < 2 or p > 10) and p != 16:
        raise ValueError(Fore.RED + "Base must be between 2,3,...,10 or 16!" + Style.RESET_ALL)
    return True

def valid_base_power_of_2(p: int) -> bool:
    """
    Verifies if the base p is a power of 2.
    For our problem statement, a base is valid if it is 2, 4, 8 or 16.

    @param p: base to be verified
    @return: True if the base is valid, False otherwise
    """
    if p != 2 and p != 4 and p != 8 and p != 16:
        return False
    return True

def remove_leading_zeros(a: str) -> str:
    """
    Removes the leading zeros from a number.
    By removing the leading zeros, the number still has the same value.
    If our number is 0, then we return 0. (special case)

    @param a: number from which the leading zeros are removed
    @return: the number without the leading zeros
    """
    for i in range(len(a)):
        if a[i] != "0":
            return a[i:]
    return "0"

def digit_to_char(digit: int) -> str:
    """
    Converts a digit to a character.
    Useful when we work with base 16.

    @param digit: digit to be converted
    @return: the character corresponding to the digit
    """
    digits = "0123456789ABCDEF"
    if digit < 0 or digit > 15:
        raise ValueError(Fore.RED + f"Digit {digit} is not a valid digit!" + Style.RESET_ALL)
    return digits[digit]

def char_to_digit(a: str) -> int:
    """
    Converts a character to a digit.
    Useful when we work with base 16.

    @param a: character to be converted
    @return: the digit corresponding to the character
    """
    digits = "0123456789ABCDEF"
    if a not in digits:
        raise ValueError(Fore.RED + f"Character {a} is not a valid digit!" + Style.RESET_ALL)
    return digits.index(a)

def base_p_to_base_10(number: str, p: int) -> int:
    """
    Converts a number from base p to base 10 by taking each digit and multiplying it by p to the power of its position.

    @param number: number to be converted
    @param p: base in which the number is
    @return: the number in base 10
    """
    result: int = 0
    for i in range(len(number)):
        result += char_to_digit(number[i]) * p ** (len(number) - i - 1)
    return result

def base_10_to_base_p(number: int, p: int) -> str:
    """
    Converts a number from base 10 to base p by dividing the number by p and concatenating the remainder at
    the beginning of the result.
    This is similar to taking the remainders in reverse order.

    @param number: number to be converted, in base 10
    @param p: base in which the number is
    @return: the number in base p
    """
    result = ""
    while number != 0:
        result = digit_to_char(number % p) + result
        number //= p
    return result

def base_p_to_base_2(a: str, p: int):
    """
    Converts a number from base p to base 2.
    Base p must be a power of 2. In our case, p can be 2, 4, 8 or 16.
    If p is 2, then the number is already in base 2, so we return it.
    If p is 4, 8 or 16, then we convert each digit to its binary representation and concatenate the results.
    We use power2 to find the number of binary digits needed to represent a digit in base p. For 4 we need 2 digits,
    for 8 we need 3 digits and for 16 we need 4 digits. This number of digits is stored in num_digits.
    At the end, we remove the leading zeros from the result.

    OBS: 1) We use the bin() function to convert a digit to its binary representation. and we remove the first 2 characters with
    [2:] because the result of bin() is a string that starts with "0b" if the number is positive or "1b" if the number is negative.
         2) We use zfill() to add leading zeros to the binary representation of a digit, so that it has num_digits digits.

    @param a: number to be converted
    @param p: base in which the number is; must be a power of 2
    @return: the number in base 2
    """
    result = ""
    power2 = 1
    num_digits = 0
    try:
        if p != 2 and p != 4 and p != 8 and p != 16:
            raise ValueError(Fore.RED + "Base must be 2, 4, 8 or 16!" + Style.RESET_ALL)
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
    """
    Converts a number from base 2 to base p.
    Base p must be a power of 2. In our case, p can be 2, 4, 8 or 16.
    If p is 2, then the number is already in base 2, so we return it.
    If p is 4, 8 or 16, then we convert each group of p digits to its base p representation and concatenate the results.
    We use power2 to find the number of binary digits needed to represent a digit in base p. For 4 we need 2 digits,
    for 8 we need 3 digits and for 16 we need 4 digits. This number of digits is stored in num_digits.
    At the end, we remove the leading zeros from the result.

    OBS: 1) We use int() with the second parameter 2 to convert a binary number to its decimal representation.
    The 2 is there to tell the interpreter that the first parameter is in base 2.
         2) We use zfill() to add leading zeros to the binary representation of a digit, so that it has the number of digits
    needed to represent a digit in base p. This bassically formats the binary number to have the desired number of digits for
    converting it into base p.

    @param a: number to be converted
    @param p: base in which the number is; must be a power of 2
    @return: the number in base p
    """
    result = ""
    power2 = 1
    num_digits = 0
    try:
        if p != 2 and p != 4 and p != 8 and p != 16:
            raise ValueError(Fore.RED + "Base must be 2, 4, 8 or 16!" + Style.RESET_ALL)
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
    return remove_leading_zeros(result)
