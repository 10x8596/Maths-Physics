import numpy as np
import sympy as sp
from scipy.linalg import lu
from scipy.linalg import null_space

A = np.array([[0,3,0,0,0],
              [-2,0,1,0,0],
              [0,-1,0,-1,0],
              [0,0,-1,0,4],
              [0,0,0,1,0]])

M = sp.Matrix([[0,3,0,0,0],
               [-2,0,1,0,0],
               [0,-1,0,-1,0],
               [0,0,-1,0,4],
               [0,0,0,1,0]])

# Matrix multiplication
# Define symbols and matrices
x1, x2, x3 = sp.symbols('x1 x2 x3')
A = sp.Matrix([[2, 3, 4],
               [3, 5, 6],
               [4, 6, 8]])
x = sp.Matrix([[x1, x2, x3]])

# Perform matrix multiplication using NumPy
result = np.dot(np.dot(x, A), x.T)
print(result)

# Calculate LU decomposition and find REF
# P is the permutation matrix, L is the lower triangular matrix
# and U is the upper triangular matrix
P, L, U = lu(A)
print("Row-Echelon Form (U matrix):")
print(U)

# calculate the reduced row-echelon form
rref, pivot_columns = M.rref()
print("Reduced Row-Echelon Form:")
print(sp.latex(rref))
# ([[1,0,0,0,-2],
#   [0,1,0,0,0],
#   [0,0,1,0,-4],
#   [0,0,0,1,0],
#   [0,0,0,0,0]])

# Find the rank of a matrix
rank_A = np.linalg.matrix_rank(A)
print("Rank of the matrix A:", rank_A)

# Determine if matrices rows and columns are linearly dependent
if rank_A < min(A.shape):
    print("The matrix is linearly dependent.")
else:
    print("The matrix is linearly independent.")

# Transpose the matrix
transposed_A = A.T
print("Transposed Matrix:")
print(transposed_A)

# Solution set x through null space of matrix A (non-invertible)
null_space_A = null_space(A)
print("The solution set for Ax=0:")
print(null_space_A)

# Calculate the moore penrose pseudo inverse of a matrix
A_pseudo_inverse = np.linalg.pinv(A)
print("The pseudo inverse of matrix A is:")
print(A_pseudo_inverse)

B = np.array([[1,2,3],
              [2,4,5],
              [3,5,6]])
squared_B = B @ B
print("B squared is:" )
print(squared_B)

X = np.array([[8,1,6],
              [3,5,7],
              [4,9,2]])
# calculate the kronecker product of matrix X
kronecker = np.kron(X, X)
print(kronecker)

C = np.array([[1, 3, 23],
              [1, 1, 6],
              [1, 1, 1]])
# Calculate the rank of matrix A
rank_C = np.linalg.matrix_rank(C)

# Determine if the matrix is injective
if rank_C == min(C.shape):
    print("The matrix is injective.")
else:
    print("The matrix is not injective.")

