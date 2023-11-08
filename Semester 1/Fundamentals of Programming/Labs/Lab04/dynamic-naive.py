from random import randint

def maxValue(A: list) -> tuple:
    """
    In the naive implementation, we take the array and each possible pair of m > n > p > q and check
    if our new formed sum is greater than the previous one. If it is, we update the current value of
    the maximum sum.
    The complexity of this algorithm is O(n^4) because we have 4 for loops, each of size n.
    """
    length: int = len(A) # Length of array
    max = float('-inf')
    for m in range(length):
        for n in range(m):
            for p in range(n):
                for q in range(p):
                    currentSum: int = A[m] - A[n] + A[p] - A[q]
                    if currentSum > max:
                        max = currentSum
                        expression: str = f"{A[m]} - {A[n]} + {A[p]} - {A[q]}"
                        
    return max, expression
                        
def main():
    A: list = []
    n = int(input("Number of elements of the list A(n>=4): "))
    A = [randint(0, 100) for i in range(n)]
        
    print(f"A = {A}")
    value, expression = maxValue(A)
    print(f"The maximum value is: {value}")
    print(f"{value} = {expression}")
    
main()