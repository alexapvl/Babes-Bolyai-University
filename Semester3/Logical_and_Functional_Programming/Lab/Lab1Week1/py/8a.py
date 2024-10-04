from lista import *

def lowest_common_multiple_recursive(a: int, b: int, i: int = 1) -> int:
    if i % a == 0 and i % b == 0:
        return i
    return lowest_common_multiple_recursive(a, b, i + 1)

def lowest_common_multiple_of_a_list_recursive(node: Nod) -> int:
    if node is None:
        return 1
    return lowest_common_multiple_recursive(node.e, lowest_common_multiple_of_a_list_recursive(node.urm))

def main():
  list = creareLista()
  print(f"Lowest common multiple is: {lowest_common_multiple_of_a_list_recursive(list.prim)}")

main()