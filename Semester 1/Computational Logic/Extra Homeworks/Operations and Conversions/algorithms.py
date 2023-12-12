# Author: Apăvăloaiei Alexandru, group 911
from utilities import *
from operations import *

def rapid_conversion(number: str, sb: int, db: int) -> str:
    """
    Algorithm which converts numbers which are in bases which are powers of 2.
    In our case, the possible bases are 2, 4, 8, 16.
    We use base 2 as an intermediary base, just like we would on paper.
    First, we convert the number to base 2, then we convert it to the destination base.

    OBS: We do not check if the bases are valid because the functions base_p_to_base_2() and base_2_to_base_p() do that
    when they are called.

    @param number: number to be converted
    @param sb: source base; must be a power of 2
    @param db: destination base; must be a power of 2
    @return: the number converted to the destination base
    """
    if sb == db:
        # number is already in the destination base
        return number
    number = base_p_to_base_2(number, sb)
    return base_2_to_base_p(number, db)

def conversion_with_intermediary_base_10(number: str, sb: int, db: int) -> str:
    """
    Algorithm which converts numbers from any base to any base by using base 10 as an intermediary base.
    First, we convert the number to base 10, then we convert it to the destination base.

    @param number: number to be converted
    @param sb: source base
    @param db: destination base
    @return: the number converted to the destination base
    """
    if sb == db:
        # number is already in the destination base
        return number
    return base_10_to_base_p(base_p_to_base_10(number, sb), db)

def substitution_method(number: str, sb: int, db: int) -> str:
    """
    Algorithm which converts numbers from a smaller base to a bigger base by using the substitution method.
    First, we create a list where we will store all the products which will be later added together to get the result.
    First for loop: We form in each element of that list the power of the source base by multiplying in the destination base.
    Second for loop: We multiply each power we formed in the first for loop with the corresponding digit from the number.
    Third for loop: We add all the products together to get the result in the destination base, because we performed all the
    operations in the destination base.

    OBS: We use the functions multiplication_by_one_digit_in_base_p() and addition_in_base_p() to perform the operations
    in the destination base.

    @param number: number to be converted
    @param sb: source base
    @param db: destination base
    @return: the number converted to the destination base
    """
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
    """
    Algorithm which converts numbers from a bigger base to a smaller base by using the successive divisions method.
    First, we divide the number by the destination base and we store the remainder at the beginning of the result.
    Second, we divide the quotient by the destination base and we store the remainder at the beginning of the result.
    We repeat this process until the quotient is 0 and then the result will be all the remainders in the order we
    concatenated them. All operations are performed in intermediary base 10. (check the function division_by_one_digit_in_base_p()
    for more details)

    OBS: We use the function division_by_one_digit_in_base_p() to perform the operations in the destination base.

    @param number: number to be converted
    @param sb: source base
    @param db: destination base
    @return: the number converted to the destination base
    """
    result = ""
    while number != "0":
        quotient, remainder = division_by_one_digit_in_base_p(number, digit_to_char(db), sb)
        number = quotient
        result = remainder + result
    return result
