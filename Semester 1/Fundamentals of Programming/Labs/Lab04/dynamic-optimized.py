from random import randint

def maxValue(A: list) -> tuple:
    """
    Args:
        A (list): List given

    Returns:
        tuple: Value(int) and expression(str)
    """
    n: int = len(A)
    max1: list = [0] * n # maximum sum of the form A[m] at index i in list A
    max2: list = [0] * n # maximum sum of the form A[m] - A[n] at index i in list A, where m > n
    max3: list = [0] * n # maximum sum of the form A[m] - A[n] + A[p] at index i in list A, where m > n > p
    max4: list = [0] * n # maximum sum of the form A[m] - A[n] + A[p] - A[q] at index i in list A, where m > n > p > q
    
    max1[n - 1] = A[n - 1] # base case
    # start for loop from n - 2 because we already have the maximum at index n - 1
    for i in range(n - 2, 2, -1):
        max1[i] = max(max1[i+1], A[i])
    
    # start for loop from n - 2 because we need an expression of the form A[m] - A[n] (2 elements)
    for i in range(n - 2, 1, -1):
        max2[i] = max(max2[i + 1], max1[i + 1] - A[i]) # "- A[i]" <=> "- A[n]" in expression
    
    # start for loop from n - 3 because we need an expression of the form A[m] - A[n] + A[p] (3 elements)
    for i in range(n - 3, 0, -1):
        max3[i] = max(max3[i + 1], max2[i + 1] + A[i]) # "+ A[i]" <=> "+ A[p]" in expression
    
    # start for loop from n - 4 because we need an expression of the form A[m] - A[n] + A[p] - A[q] (4 elements)
    for i in range(n - 4, -1, -1):
        max4[i] = max(max4[i + 1], max3[i + 1] - A[i]) # "- A[i]" <=> "- A[q]"" in expression
    
    # determine index4 -> the highest position of the maximum(until index n-3 because we still 
    # have values for index3, index2 and index1 to be found)
    highValue =  float('-inf')
    for i in range(0, n - 3):
        if max4[i] >= highValue: # ">=" because we want the highes position, not the first occured
            index4 = i
            highValue = max4[i]
    
    # determine index3 -> the highest position of the maximum, which is relative to index4, 
    # because it needs to be STRICTLY greater than index4 (until index n-2 because we still need 
    # to have values for index2 and index1 to be found)
    highValue =  float('-inf') # reset value of highValue
    for i in range(index4 + 1, n - 2):
        if max3[i] >= highValue:
            index3 = i
            highValue = max3[i]
            
    # determine index2 -> the highest position of the maximum, which is relative to index3, 
    # because it needs to be STRICTLY greater than index3 (until index n-1 because we still need 
    # to have values for index1 to be found)
    highValue =  float('-inf') # reset value of highValue
    for i in range(index3 + 1, n-1):
        if max2[i] >= highValue:
            index2 = i
            highValue = max2[i]
            
    # determine index1 -> the highest position of the maximum, which is relative to index2, 
    # because it needs to be STRICTLY greater than index2(we go until the end of list)
    highValue =  float('-inf') # reset value of highValue
    for i in range(index2 + 1, n):
        if max1[i] >= highValue:
            index1 = i
            highValue = max1[i]
        
    return max4[0], f"{A[index1]} - {A[index2]} + {A[index3]} - {A[index4]}"
    
    
def main():
    A: list = []
    n = int(input("Number of elements of the list A (n>=4): "))
    A = [randint(0, 100) for i in range(n)]
    print(f"A = {A}")

    value, expression = maxValue(A)
    print(f"The maximum value is: {value}")
    print(f"{value} = {expression}")
    
     
main()