# Problem 6

def backtracking_iterative(n: int):
    # The time complexity of this algorithm is O(2^n) and it is calculated in the same way as the recursive one
    list = []
    list.append(['', 0, 0])
    while list:
        x, opened, closed = list.pop()
        if len(x) == n:
            print(x)
        else:
            if opened < n / 2:
                list.append([x + '(', opened + 1, closed])
            if closed < opened:
                list.append([x + ')', opened, closed + 1])
        #print(stack) # Uncomment this line to see the stack progression

def main():              
    #call function and display results
    n = int(input("Desired length: "))
    if n % 2 == 1:
        print("Invalid input. The value of n needs to be even!")
    else:
        backtracking_iterative(n)
        
main()
