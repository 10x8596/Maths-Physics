import numpy as np
import sympy as sp

# Formulate the Linear System of a square matrix
def solve_linear_system(A, b):
    # Check if A is singular. If it is, try Least Squares Solution
    try:
        # Find the Particular Solution
        x = np.linalg.solve(A, b)
        print("Particular solution found using np.linalg.solve.")
    except np.linalg.LinAlgError:
        print("Matrix is singular; using np.linalg.lstsq instead.")
        x, residuals, rank, s = np.linalg.lstsq(A, b, rcond=None)

    # Find the Null Space (Using SVD)
    U, S, Vt = np.linalg.svd(A)
    # Columns of V corresponding to zero singular values
    try:
        null_space = Vt.T[:, S < 1e-10]
    except IndexError:
        print("boolean index did not match indexed array\n"
              " dimension is", A.shape[1], "but corresponding\n"
              " dimension is", b.shape[0])

    # Generate the General Solution  
    try:
        if null_space.size == 0:
            # No null space, only the particular solution
            general_solution = x[:, None]
        else:
            # Combine the particular solution and the null space
            general_solution = x[:, None] +\
                            null_space @\
                            np.ones((null_space.shape[1], b.shape[1]))
    except UnboundLocalError:
        print("cannot access local variable 'null_space'\n"
              " where it is not associated with a value")
    try:
        return x, null_space, general_solution, residuals, rank, s
    except UnboundLocalError:
        print("unboundlocalerror")

A = np.array([[1, 2, -1, 3],
              [2, 4, -2, 6],
              [3, 6, -3, 9],
              [4, 8, -4, 11]])

b = np.array([[1], [2], [3], [5]])

solution = solve_linear_system(A, b)

print("Particular solution:")
sp.pprint(solution[0])
print("Null Space Basis Vectors:")
sp.pprint(solution[1])
print("General solution:")
sp.pprint(solution[2])

B = np.array([[4,3,2,2,-2],
              [0,1,2,2,6],
              [3,2,1,1,-3],
              [-1,0,1,1,1]])
print(B)

a = np.array([[5], [23], [-2], [16]])

solution = solve_linear_system(B, a)

#print("Solution x:\n", solution[0])
# print("Residuals:\n", solution[3])
# print("Rank of B:", solution[4])
# print("Singular values of B:\n", solution[5])
