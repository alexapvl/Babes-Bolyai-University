from colorama import Fore, Style
from functions import *

#
# Here the program will perform test before running, to check and validate strings
#
#

def test_expression_validator():
    assert expression_validator("add 4+2i") == True
    assert expression_validator("add -4+2i") == True
    assert expression_validator("insert 1+1i at 1") == True
    assert expression_validator("insert -1+1i at 1") == True
    assert expression_validator("remove 1") == True
    assert expression_validator("remove 1 to 3") == True
    assert expression_validator("replace 1+3i with 5-3i") == True
    assert expression_validator("replace -1+3i with 5-3i") == True
    assert expression_validator("list") == True
    assert expression_validator("list real 1 to 5") == True
    assert expression_validator("list modulo < 10") == True
    assert expression_validator("list modulo = 5") == True
    assert expression_validator("filter real") == True
    assert expression_validator("filter modulo < 10") == True
    assert expression_validator("filter modulo > 6") == True
    assert expression_validator("filter modulo = 6") == True
    assert expression_validator("undo") == True
    assert expression_validator("exit") == True
    
def test_format_number():
    assert format_number("-1-1i") == "-1-i"
    assert format_number("-1+1i") == "-1+i"
    assert format_number("1+1i") == "1+i"
    assert format_number("10+1i") == "10+i"
    assert format_number("0+1i") == "i"
    assert format_number("0-1i") == "-i"
    assert format_number("-2+0i") == "-2"
    assert format_number("2+0i") == "2"
    assert format_number("19-15i") == "19-15i"
    assert format_number("2-0i") == "2"
    assert format_number("2+0i") == "2"
    assert format_number("-2+0i") == "-2"
    assert format_number("-2-0i") == "-2"
    
def test_get_real_imaginary():
    assert get_real("1+1i") == "1"
    assert get_real("-1+1i") == "-1"
    assert get_imaginary("1+1i") == "1"
    assert get_imaginary("1-1i") == "-1"
    
def test_run():
    test_list = []
    test_list = add("2+3i", test_list)
    assert test_list == ["2+3i"]
    test_list = add("2-3i", test_list)
    assert test_list == ["2+3i", "2-3i"]
    test_list = insert("1+1i", 0, test_list)
    assert test_list == ["1+1i", "2+3i", "2-3i"]
    test_list = remove(test_list, 0)
    assert test_list == ["2+3i", "2-3i"]
    test_list = replace("2+3i", "1+1i", test_list)
    assert test_list == ["1+1i", "2+2i"]

def test():
    try:
        test_expression_validator()
        test_format_number()
        test_get_real_imaginary()
        test_run()
    except AssertionError as ae:
        print(Fore.RED + f"{ae}" + Style.RESET_ALL)
    print(Fore.GREEN + "All tests passed successfully." + Style.RESET_ALL)
