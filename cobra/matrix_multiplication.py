import numpy as np
import sympy as sp
from scipy.linalg import lu
from scipy.linalg import null_space

# function to compute the cube of a matrix
def cube_matrix(matrix):
    return np.linalg.matrix_power(matrix, 3)

def matrix_multiply(A, x):
    return A @ x

A = np.array([[1,-1,2,3,-1],
              [2,-2,4,6,-2],
              [3,-3,6,9,-3],
              [4,-4,8,11,-4],
              [5,-5,10,14,-5]])

M = sp.Matrix([[0,3,0,0,0],
               [-2,0,1,0,0],
               [0,-1,0,-1,0],
               [0,0,-1,0,4],
               [0,0,0,1,0]])

matrix = sp.Matrix([[1+2-(2*3)+5],
                    [2],
                    [3],
                    [0],
                    [5]])

# Perform matrix cubing
# result = cube_matrix(matrix)
# print(result)

# perform matrix multiplication
result = matrix_multiply(A, matrix)
print(result)
