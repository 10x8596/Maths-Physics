import numpy as np

# # Determine whether an inverse exists for a matrix
# from sympy import symbols, Matrix, solve

# # Redefine the variables and matrix A
# a, b, c = symbols('a b c')
# A = Matrix([[1, 1, b], [1, a, c], [1, 1, 1]])

# # Calculate the determinant of A
# determinant = A.det()
# print(determinant)

# # Solve the determinant
# # define the variables
# a, b = symbols('a b')
# # Define the equation
# equation = -a*b + a + b - 1

# # Solve the equation for a in terms of b
# solution_a = solve(equation, a)
# print(solution_a)

A = np.array([[15, 1], [5, 10], [8, 2], [11, 19]])
B = np.array([[18], [15], [2], [8]])

def pseudo_inverse(A):
    # TODO: find the pseudo inverse of a matrix
    return np.linalg.inv(A.T @ A) @ A.T

def solve_with_pseudo_inverse(A, B):
    A_pseudo = pseudo_inverse(A)
    # TODO: solve for x using pseudo inverse method.
    X = A_pseudo @ B
    return X

X = solve_with_pseudo_inverse(A, B)
    
print(pseudo_inverse)
print(X)