from colorama import Fore, Style

def verify_number_in_base_p(a: str, p: int) -> bool:
    if (p < 2 or p > 10) and p != 16:
        raise ValueError(Fore.RED + "p must be between 2 and 16!" + Style.RESET_ALL)
    digits = "0123456789ABCDEF"
    try:
        for i in range(len(a)):
            if digits.index(a[i]) >= p:
                return False
    # in case a[i] is not in digits
    except ValueError as ve:
        print(Fore.RED + "Digit out of range!" + Style.RESET_ALL)
        return False
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
    if not verify_number_in_base_p(number, p):
        raise ValueError(Fore.RED + "Invalid number!" + Style.RESET_ALL)
    result: int = 0
    for i in range(len(number)):
        result += char_to_digit(number[i]) * p ** (len(number) - i - 1)
    return result
