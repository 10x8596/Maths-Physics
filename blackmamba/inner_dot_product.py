import numpy as np
import sympy as sp
from scipy.linalg import lu
from scipy.linalg import null_space

# inner product
# Define symbols and matrices
x1, x2, x3 = sp.symbols('x1 x2 x3')
A = sp.Matrix([[2, 3, 4],
               [3, 5, 6],
               [4, 6, 8]])
x = sp.Matrix([[x1, x2, x3]])

# Perform inner product calculation
result = np.dot(np.dot(x, A), x.T)
print(result)



def function(arr):
    arr.sort(reverse=True)
    k = 1
    for elem in arr:
        if elem >= k:
            k += 1
        else:
            return k-1

C = [5,2,6,1,8,4]
result = function(C)
print(result)