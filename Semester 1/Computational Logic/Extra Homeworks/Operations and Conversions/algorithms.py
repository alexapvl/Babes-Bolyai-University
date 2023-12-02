# Author: Apăvăloaiei Alexandru, group 911
from utilities import *
from operations import *

def rapid_conversion(number: str, sb: int, db: int) -> str:
    if sb == db == 2:
        # number is already in base 2
        return number
    number = base_p_to_base_2(number, sb)
    return base_2_to_base_p(number, db)

def conversion_with_intermediary_base_10(number: str, sb: int, db: int) -> str:
    if sb == db == 10:
        # number is already in base 10
        return number
    return base_10_to_base_p(base_p_to_base_10(number, sb), db)

def substitution_method(number: str, sb: int, db: int) -> str:
    list_of_products_in_dest_base = ["1"] * len(number)
    for i in range(len(number)):
        power = len(number) - i - 1
        for p in range(power):
            list_of_products_in_dest_base[i] = multiplication_by_one_digit_in_base_p(list_of_products_in_dest_base[i], digit_to_char(sb), db)
    for i in range(len(number)):
        list_of_products_in_dest_base[i] = multiplication_by_one_digit_in_base_p(list_of_products_in_dest_base[i], number[i], db)
    result = "0"
    for i in range(len(number)):
        result = addition_in_base_p(result, list_of_products_in_dest_base[i], db)
    return result

def successive_divisions_method(number: str, sb: int, db: int) -> str:
    result = ""
    while number != "0":
        quotient, remainder = division_by_one_digit_in_base_p(number, digit_to_char(db), sb)
        number = quotient
        result = remainder + result
    return result
