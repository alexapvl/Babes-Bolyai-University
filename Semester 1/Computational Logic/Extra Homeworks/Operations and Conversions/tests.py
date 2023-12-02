# Author: Apăvăloaiei Alexandru, group 911
import operations as op
import utilities as utils
import algorithms as alg
from colorama import Fore, Style

def base_p_to_base_10_test():
    assert utils.base_p_to_base_10("123", 10) == 123
    assert utils.base_p_to_base_10("100", 2) == 4
    assert utils.base_p_to_base_10("1000", 2) == 8
    assert utils.base_p_to_base_10("123", 7) == 66
    assert utils.base_p_to_base_10("23", 9) == 21
    assert utils.base_p_to_base_10("123", 9) == 102
    assert utils.base_p_to_base_10("501BA", 16) == 328122
    assert utils.base_p_to_base_10("ABCDEF", 16) == 11259375

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

def remove_leading_zeros_test():
    assert utils.remove_leading_zeros("00000") == "0"
    assert utils.remove_leading_zeros("00001") == "1"
    assert utils.remove_leading_zeros("00021") == "21"
    assert utils.remove_leading_zeros("00ABC") == "ABC"

def verify_number_in_base_p_test():
    assert utils.verify_number_in_base_p("123", 10) == True
    assert utils.verify_number_in_base_p("123", 7) == True
    assert utils.verify_number_in_base_p("123", 4) == True
    assert utils.verify_number_in_base_p("ABCDEF", 16) == True

def base_2_to_base_p_test():
    # base 2 to base 4
    assert utils.base_2_to_base_p("1011", 4) == "23"
    assert utils.base_2_to_base_p("11110", 4) == "132"
    assert utils.base_2_to_base_p("110001", 4) == "301"
    #base 2 to base 8
    assert utils.base_2_to_base_p("100101", 8) == "45"
    assert utils.base_2_to_base_p("1010111", 8) == "127"
    assert utils.base_2_to_base_p("110100", 8) == "64"
    #base 2 to base 16
    assert utils.base_2_to_base_p("11010", 16) == "1A"
    assert utils.base_2_to_base_p("11110011", 16) == "F3"
    assert utils.base_2_to_base_p("101011", 16) == "2B"

def base_p_to_base_2_test():
    # base 4 to base 2
    assert utils.base_p_to_base_2("023", 4) == "1011"
    assert utils.base_p_to_base_2("132", 4) == "11110"
    assert utils.base_p_to_base_2("301", 4) == "110001"
    # base 8 to base 2
    assert utils.base_p_to_base_2("045", 8) == "100101"
    assert utils.base_p_to_base_2("127", 8) == "1010111"
    assert utils.base_p_to_base_2("64", 8) == "110100"
    # base 16 to base 2
    assert utils.base_p_to_base_2("01A", 16) == "11010"
    assert utils.base_p_to_base_2("F3", 16) == "11110011"
    assert utils.base_p_to_base_2("2B", 16) == "101011"

def test_utilities():
    base_p_to_base_10_test()
    digit_to_char_test()
    char_to_digit_test()
    remove_leading_zeros_test()
    verify_number_in_base_p_test()
    base_2_to_base_p_test()
    base_p_to_base_2_test()

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

def multiplication_test():
    assert op.multiplication_by_one_digit_in_base_p("7023", "5", 8) == "43137"
    assert op.multiplication_by_one_digit_in_base_p("12345", "5", 7) == "65424"
    assert op.multiplication_by_one_digit_in_base_p("12345", "5", 6) == "111101"
    assert op.multiplication_by_one_digit_in_base_p("31203", "3", 5) == "144114"
    assert op.multiplication_by_one_digit_in_base_p("31203", "3", 4) == "220221"
    assert op.multiplication_by_one_digit_in_base_p("21563", "7", 8) == "174045"
    assert op.multiplication_by_one_digit_in_base_p("21563", "0", 8) == "0"
    # test commutativity
    assert op.multiplication_by_one_digit_in_base_p("2", "72", 8) == op.multiplication_by_one_digit_in_base_p("72", "2", 8)
    assert op.multiplication_by_one_digit_in_base_p("2", "72", 7) == op.multiplication_by_one_digit_in_base_p("72", "2", 7)

def division_test():
    assert op.division_by_one_digit_in_base_p("AF", "B", 16) == ("F", "A")
    assert op.division_by_one_digit_in_base_p("1FED0205", "9", 16) == ("38C1CAB", "2")
    assert op.division_by_one_digit_in_base_p("120456", "6", 7) == ("13421", "0")
    assert op.division_by_one_digit_in_base_p("321023", "3", 5) == ("103322", "2")
    assert op.division_by_one_digit_in_base_p("321023", "3", 4) == ("103003", "2")
    assert op.division_by_one_digit_in_base_p("2A0F86", "E", 16) == ("3011B", "C")
    assert op.division_by_one_digit_in_base_p("98776431", "4", 10) == ("24694107", "3")

def test_operations():
    addition_test()
    subtraction_test()
    multiplication_test()
    division_test()

def rapid_conversion_test():
    assert alg.rapid_conversion("10000001101100", 2, 8) == "20154"
    assert alg.rapid_conversion("11110010000001001010", 2, 16) == "F204A"
    assert alg.rapid_conversion("11024", 8, 2) == "1001000010100"
    assert alg.rapid_conversion("BC13F", 16, 2) == "10111100000100111111"
    assert alg.rapid_conversion("1230", 4, 8) == "154"
    assert alg.rapid_conversion("AB650", 16, 8) == "2533120"
    assert alg.rapid_conversion("1C3D", 16, 4) == "1300331"
    assert alg.rapid_conversion("63401", 8, 16) == "6701"

def substitution_method_test():
    assert alg.substitution_method("4321", 5, 8) == "1112"
    assert alg.substitution_method("3210", 4, 10) == "228"
    assert alg.substitution_method("3041", 5, 10) == "396"
    assert alg.substitution_method("1735", 8, 10) == "989"
    assert alg.substitution_method("10111010011", 2, 10) == "1491"
    assert alg.substitution_method("2345", 6, 16) == "239"
    assert alg.substitution_method("323415", 6, 16) == "6863"
    assert alg.substitution_method("452631", 7, 16) == "1396B"
    assert alg.substitution_method("137265", 8, 16) == "BEB5"
    assert alg.substitution_method("173840", 9, 16) == "1A547"
    assert alg.substitution_method("987654", 10, 16) == "F1206"

def successive_divisions_method_test():
    assert alg.successive_divisions_method("643", 7, 5) == "2300"
    assert alg.successive_divisions_method("ABC", 16, 7) == "11004"
    assert alg.successive_divisions_method("3842", 10, 5) == "110332"
    assert alg.successive_divisions_method("10379", 10, 7) == "42155"
    assert alg.successive_divisions_method("2653", 10, 6) == "20141"
    assert alg.successive_divisions_method("222", 10, 2) == "11011110"
    assert alg.successive_divisions_method("1467", 8, 5) == "11243"
    assert alg.successive_divisions_method("2510", 7, 3) == "1021202"
    assert alg.successive_divisions_method("7048", 9, 4) == "1100123"
    assert alg.successive_divisions_method("BC0D", 16, 6) == "1010513"

def conversion_with_intermediary_base_10_test():
    # same tests as rapid conversion test
    assert alg.conversion_with_intermediary_base_10("10000001101100", 2, 8) == "20154"
    assert alg.conversion_with_intermediary_base_10("11110010000001001010", 2, 16) == "F204A"
    assert alg.conversion_with_intermediary_base_10("11024", 8, 2) == "1001000010100"
    assert alg.conversion_with_intermediary_base_10("BC13F", 16, 2) == "10111100000100111111"
    assert alg.conversion_with_intermediary_base_10("1230", 4, 8) == "154"
    assert alg.conversion_with_intermediary_base_10("AB650", 16, 8) == "2533120"
    assert alg.conversion_with_intermediary_base_10("1C3D", 16, 4) == "1300331"
    assert alg.conversion_with_intermediary_base_10("63401", 8, 16) == "6701"
    # same tests as substitution_method_test
    assert alg.conversion_with_intermediary_base_10("4321", 5, 8) == "1112"
    assert alg.conversion_with_intermediary_base_10("3210", 4, 10) == "228"
    assert alg.conversion_with_intermediary_base_10("3041", 5, 10) == "396"
    assert alg.conversion_with_intermediary_base_10("1735", 8, 10) == "989"
    assert alg.conversion_with_intermediary_base_10("10111010011", 2, 10) == "1491"
    assert alg.conversion_with_intermediary_base_10("2345", 6, 16) == "239"
    assert alg.conversion_with_intermediary_base_10("323415", 6, 16) == "6863"
    assert alg.conversion_with_intermediary_base_10("452631", 7, 16) == "1396B"
    assert alg.conversion_with_intermediary_base_10("137265", 8, 16) == "BEB5"
    assert alg.conversion_with_intermediary_base_10("173840", 9, 16) == "1A547"
    assert alg.conversion_with_intermediary_base_10("987654", 10, 16) == "F1206"
    # same tests as successive_divisions_method_test
    assert alg.conversion_with_intermediary_base_10("643", 7, 5) == "2300"
    assert alg.conversion_with_intermediary_base_10("ABC", 16, 7) == "11004"
    assert alg.conversion_with_intermediary_base_10("3842", 10, 5) == "110332"
    assert alg.conversion_with_intermediary_base_10("10379", 10, 7) == "42155"
    assert alg.conversion_with_intermediary_base_10("2653", 10, 6) == "20141"
    assert alg.conversion_with_intermediary_base_10("222", 10, 2) == "11011110"
    assert alg.conversion_with_intermediary_base_10("1467", 8, 5) == "11243"
    assert alg.conversion_with_intermediary_base_10("2510", 7, 3) == "1021202"
    assert alg.conversion_with_intermediary_base_10("7048", 9, 4) == "1100123"
    assert alg.conversion_with_intermediary_base_10("BC0D", 16, 6) == "1010513"

def test_algorithms():
    rapid_conversion_test()
    substitution_method_test()
    successive_divisions_method_test()
    conversion_with_intermediary_base_10_test()

def test():
    try:
        test_utilities()
        test_operations()
        test_algorithms()
        print(Fore.GREEN + "All tests passed!" + Style.RESET_ALL)
    except AssertionError:
        print(Fore.RED + "Some tests failed!" + Style.RESET_ALL)
    except ValueError as ve:
        print(Fore.RED + str(ve) + Style.RESET_ALL)
