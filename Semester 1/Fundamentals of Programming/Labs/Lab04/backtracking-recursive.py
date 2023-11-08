# Problem 6

"""
When backtracking, we need to check that at all times the number of open parantheses is greater or equal to
the number of closed parantheses. If at the end the number of total parantheses is equal to n, the first 
paranthesis is open "(", the last is closed ")" and we print the solution if these statements are satisfied.
"""
def backtracking_recursive(n:int, x: str, opened: int, closed: int):
    # The time complexity of this algorithm is O(2^n) because we have 2 choices at each step
    # and we calculate it for n steps (n being the length of the string) 
    if len(x) == n:
        print(x)  # When we find a valid solution
    else:
        if opened < n / 2:
            # print(x) # Uncomment this to see the stack progression
            backtracking_recursive(n, x + '(', opened + 1, closed) # We add an open paranthesis
        if closed < opened:
            # print(x) # Uncomment this to see the stack progression
            backtracking_recursive(n, x + ')', opened, closed + 1) # We add a closed paranthesis

def main():
    n = int(input("Desired length: "))

    if n % 2 == 1:
        print("Invalid input. The value of n needs to be even!")
    else:
        backtracking_recursive(n, '', 0, 0)
        
main()