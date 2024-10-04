from lista import *

def substitute_recursive(node: Nod, value: int, substitution: int) -> None:
    if node is None:
        return
    if node.e == value:
        node.e = substitution
    substitute_recursive(node.urm, value, substitution)

def main():
  list = creareLista()
  # print(f"Lowest common multiple is: {lowest_common_multiple_of_a_list_recursive(list.prim)}")
  print("Enter the value you want to substitute:")
  value = int(input())
  print("Enter the value you want to substitute with:")
  substitution = int(input())
  substitute_recursive(list.prim, value, substitution)
  tipar(list)
  
main()