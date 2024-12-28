import numpy as np
from numpy.linalg import det

def is_positive_definite(matrix):
    # Check if matrix is symmetric
    if not np.allclose(matrix, matrix.T):
        return False, "Matrix is not symmetric"
    
    # Eigenvalue check
    eigenvalues = np.linalg.eigvals(matrix)
    print(eigenvalues)
    if np.all(eigenvalues > 0):
        return True, "Matrix is positive definite"
    else:
        return False, "Matrix is not positive definite"
    
def is_positive_semidefinite(matrix):
    # Check if the matrix is symmetric
    if not np.allclose(matrix, matrix.T):
        return False

    # Check the leading principal minors
    n = matrix.shape[0]
    for i in range(1, n + 1):
        principal_minor = matrix[:i, :i]
        if det(principal_minor) < 0:  # Changed from <= to <
            return False
    
    return True
    
matrix = np.array([[2,3,4],
                  [3,5,6],
                  [4,6,8]])

result, message = is_positive_definite(matrix)
print(message)
is_psd = is_positive_semidefinite(matrix)
print(is_psd)