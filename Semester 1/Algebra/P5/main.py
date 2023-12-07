import numpy as np
import sympy as sp
import itertools as it
import sys

def generate_matrices(m: int, n: int) -> list:
    # generates a list of all possible matrices of dimension m * n with elements from Z_2
    return np.array(list(it.product([0,1],repeat=m * n)))

def generate_set_of_echalon_form_matrices(matrices: list) -> set:
    result_set = set()
    for matrix in matrices:
        # convert from a list to a matrix
        matrix = matrix.reshape(m, n)
        # convert from a numpy matrix to a sympy matrix
        matrix = sp.Matrix(matrix)
        # get the row echalon form of the matrix
        matrix = matrix.rref()[0].tolist()
        # verify if the matrix is in Z_2
        ok = True
        for line in matrix:
            for element in line:
                if element != 1 and element != 0:
                    ok = False
        if ok:
            # if the matrix is in Z_2, makes it hashable and adds it to the set
            result_set.add(tuple(map(tuple,matrix)))
    return result_set

if __name__ == "__main__":
    with open("input5.txt", "r") as f:
        m = int(f.readline())
        n = int(f.readline())
    matrices = generate_matrices(m, n)
    result = generate_set_of_echalon_form_matrices(matrices)
    with open("output5.txt", "w") as f:
        f.write(f"The number of matrices of dimension {m} * {n} with elements from Z_2 in row echalon form is {len(result)}.\n")
        f.write("The matrices are:\n")
        for matrix in result:
            f.write(str(matrix) + "\n")

    
    