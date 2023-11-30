# When importing, import as "import tests as t" to have more clarity
# in the code

import operations as op
import utilities as utils
from colorama import Fore, Style

#TODO: test base_p_to_base_10
def base_p_to_base_10_test():
    assert utils.base_p_to_base_10("123", 10) == 123
    assert utils.base_p_to_base_10("501BA", 16) == 328122
    assert utils.base_p_to_base_10("ABCDEF", 16) ==11259375

def digit_to_char_test():
    assert utils.digit_to_char(0) == "0"
    assert utils.digit_to_char(1) == "1"
    assert utils.digit_to_char(2) == "2"
    assert utils.digit_to_char(3) == "3"
    assert utils.digit_to_char(4) == "4"
    assert utils.digit_to_char(5) == "5"
    assert utils.digit_to_char(6) == "6"
    assert utils.digit_to_char(7) == "7"
    assert utils.digit_to_char(8) == "8"
    assert utils.digit_to_char(9) == "9"
    assert utils.digit_to_char(10) == "A"
    assert utils.digit_to_char(11) == "B"
    assert utils.digit_to_char(12) == "C"
    assert utils.digit_to_char(13) == "D"
    assert utils.digit_to_char(14) == "E"
    assert utils.digit_to_char(15) == "F"

def char_to_digit_test():
    assert utils.char_to_digit("0") == 0
    assert utils.char_to_digit("1") == 1
    assert utils.char_to_digit("2") == 2
    assert utils.char_to_digit("3") == 3
    assert utils.char_to_digit("4") == 4
    assert utils.char_to_digit("5") == 5
    assert utils.char_to_digit("6") == 6
    assert utils.char_to_digit("7") == 7
    assert utils.char_to_digit("8") == 8
    assert utils.char_to_digit("9") == 9
    assert utils.char_to_digit("A") == 10
    assert utils.char_to_digit("B") == 11
    assert utils.char_to_digit("C") == 12
    assert utils.char_to_digit("D") == 13
    assert utils.char_to_digit("E") == 14
    assert utils.char_to_digit("F") == 15

def verify_leading_zeros_test():
    assert utils.remove_leading_zeros("00000") == "0"
    assert utils.remove_leading_zeros("00001") == "1"
    assert utils.remove_leading_zeros("00021") == "21"
    assert utils.remove_leading_zeros("00ABC") == "ABC"

def verify_number_in_base_p_test():
    assert utils.verify_number_in_base_p("123", 10) == True
    assert utils.verify_number_in_base_p("123", 7) == True
    assert utils.verify_number_in_base_p("123", 4) == True
    assert utils.verify_number_in_base_p("123", 3) == False
    assert utils.verify_number_in_base_p("123", 2) == False
    assert utils.verify_number_in_base_p("ABCDEF", 16) == True

def addition_test():
    assert op.addition_in_base_p("123", "456", 10) == "579"
    assert op.addition_in_base_p("123", "456", 7) == "612"
    assert op.addition_in_base_p("54AB6F", "CD097D", 16) == "121B4EC"
    # test commutativity
    assert op.addition_in_base_p("123", "456", 10) == op.addition_in_base_p("456", "123", 10)
    assert op.addition_in_base_p("123", "456", 8) == op.addition_in_base_p("456", "123", 8)
    assert op.addition_in_base_p("54AB6F", "CD097D", 16) == op.addition_in_base_p("CD097D", "54AB6F", 16)

def subtraction_test():
    assert op.subtraction_in_base_p("10001100010", "1110111011", 2) == "10100111"
    assert op.subtraction_in_base_p("102387", "64502", 9) == "26785"
    assert op.subtraction_in_base_p("130046", "71257", 8) == "36567"
    assert op.subtraction_in_base_p("210354", "55466", 7) == "121555"
    assert op.subtraction_in_base_p("210354", "44355", 6) == "121555"
    assert op.subtraction_in_base_p("102003", "3333", 5) == "43120"
    assert op.subtraction_in_base_p("102003", "3333", 4) == "32010"
    assert op.subtraction_in_base_p("100111000", "1100111", 2) == "11010001"
    assert op.subtraction_in_base_p("501BA", "32ED", 16) == "4CECD"
    assert op.subtraction_in_base_p("ABCD5678", "12345678", 16) == "99990000"
    assert op.subtraction_in_base_p("ABCD5678", "1234567F", 16) == "9998FFF9"

def multiplication_by_one_digit_test():
    assert op.multiplication_by_one_digit_in_base_p("7023", "5", 8) == "43137"
    assert op.multiplication_by_one_digit_in_base_p("12345", "5", 7) == "65424"
    assert op.multiplication_by_one_digit_in_base_p("12345", "5", 6) == "111101"
    assert op.multiplication_by_one_digit_in_base_p("31203", "3", 5) == "144114"
    assert op.multiplication_by_one_digit_in_base_p("31203", "3", 4) == "220221"
    assert op.multiplication_by_one_digit_in_base_p("21563", "7", 8) == "174045"
    # test commutativity
    assert op.multiplication_by_one_digit_in_base_p("2", "72", 8) == op.multiplication_by_one_digit_in_base_p("72", "2", 8)
    assert op.multiplication_by_one_digit_in_base_p("2", "72", 7) == op.multiplication_by_one_digit_in_base_p("72", "2", 7)

def test():
    try:
        base_p_to_base_10_test()
        digit_to_char_test()
        char_to_digit_test()
        verify_leading_zeros_test()
        verify_number_in_base_p_test()
        addition_test()
        subtraction_test()
        multiplication_by_one_digit_test()
        print(Fore.GREEN + "All tests passed!" + Style.RESET_ALL)
    except AssertionError:
        print(Fore.RED + "Some tests failed!" + Style.RESET_ALL)
    except ValueError as ve:
        print(Fore.RED + str(ve) + Style.RESET_ALL)

test()
