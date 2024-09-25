# Creating a Gaussian Elimination Algorithm from scratch to solve systems of linear equations

import numpy as np
import sympy as sp

def forward_elimination(res, rows, cols):
    """
    This function finds the maximum value of a column
    and makes it the pivot of the column through row swapping
    as well as transforms matrix into upper-triangle form

    Parameters:
    - res: A matrix
    - rows: the number of rows in res
    - cols: the number of cols in res
    """
    row, col = 0, 0
    while row < rows and col < cols:
        # Loop through entire matrix and swap rows
        # Find the maximum value in the current column.
        # This will be the pivot of the column.
        pivot_col = np.argmax(np.abs(res[row:, col])) + row
        
        # If the maximum value is zero (column of zeros), 
        # continue to the next column
        if abs(res[pivot_col, col]) < 1e-10:
            col += 1
            continue
        # swap current row with row with max value in pivot
        if pivot_col != row:
            res[[row, pivot_col]] = res[[pivot_col, row]]
            
        for r in range(row + 1, rows):
            factor = res[r, col] / res[row, col]
            res[r, col:] -= factor * res[row, col:]
        # Increment the rows and columns to move on
        row += 1
        col += 1
    return res

def backward_elimination(res, rows, cols):
    """
    This function tranforms a upper-triangle matrix to rref.
    Parameters:
    - res: the upper-triangle form matrix
    - rows: the number of rows in res
    - cols: the number of cols in res
    """
    # Find factor
    for row in range(min(rows, cols)-1, -1, -1):
        for next_row in range(row-1, -1, -1):
            if res[row, row] != 0:
                factor = res[next_row, row] / res[row, row]
                # make all values above pivot zero
                res[next_row, row:] -= factor * res[row, row:]
    return res
    
def make_pivots_one(res, rows, cols):
    """
    This function normalizes the pivots of a reduced matrix to 1

    Parameters:
    - res: the reduced matrix
    - rows: the number of rows in res
    - cols: the number of cols in res
    """
    # make diagonals one
    for row in range(min(rows, cols)):
        pivot = res[row, row]
        if pivot != 0:
            res[row] /= pivot
    return res

def gaussian_elim(X):
    '''
    This function uses the Gaussian elimination algorithm to reduce
    a matrix to its row reduced echelon form
    
    Parameters:
    - X: The matrix to row-reduce
    
    Returns:
    - A matrix in row-reduced echelon form
    '''
    rows, cols = X.shape
    res = np.array(X, dtype=np.float64)
    # TODO: implement Gaussian Elimination on any matrix.
    # print(f"The matrix to row-reduce is:\n {X}")
    forward_elimination(res, rows, cols)
    res[abs(res) < 1e-10] = 0
    # print(f"The matrix after forward elim is:\n {res}")
    backward_elimination(res, rows, cols)
    # res[abs(res) < 1e-10] = 0
    # print(f"The matrix after backward elim is:\n {res}")
    make_pivots_one(res, rows, cols)
    # res[abs(res) < 1e-10] = 0
    # print(f"The matrix after normalizing is:\n {res}")
    # print(f"the row reduced matrix using sympy is:\n {sp.Matrix(X).rref()}")
    # print(f"the row reduced matrix using gauss elim is:\n {res}\n")
    # Do not change the following line
    res[abs(res) < 1e-10] = 0
    return res


def find_pivots_and_free_vars(rref_A, rows, cols):
    """
    This function identifies the pivots columns and free variables
    
    Parameters
    - rref_A: rref matrix [A|0 or b]
    - rows: number of rows in rref_A
    - cols: number of cols in rref_A

    Returns:
    - pivot_columns: list of pivot columns
    - free_vars: list of free variables
    """
    # Identify pivot columns
    pivot_columns = [] # indexes of columns that are pivots
    for i, row in enumerate(rref_A):
        for col, element in enumerate(row[:-1]):
            if abs(element - 1) < 1e-10:
                pivot_columns.append(col)
                break
    # print(f"Identified pivot columns: {pivot_columns}")
    # Identify free variables (columns that do not have leading ones
    # (pivots))
    free_vars = [i for i in range(cols) if i not in pivot_columns]
    # print(f"The free variables are:\n {free_vars}")
    return pivot_columns, free_vars

def linalg_solve(rref_A, rows, cols):
    """
    This function solves systems of linear equations

    Parameters
    - rref_A: rref matrix [A|0 or b]
    - rows: number of rows in rref_A
    - cols: number of cols in rref_A
    Returns:
    - res: solved linear system
    """
    pivot_cols, free_vars = find_pivots_and_free_vars(rref_A, rows, cols)

    basis_null_space = np.array([])
    # for each free var (non pivot col), form a basis vector
    for free_var in free_vars:
        basis_vector = np.zeros(cols) # fill with zeros
        basis_vector[free_var] = 1 # set free var to 1 to solve basic variables

        # Loop through to populate the entries using the 
        # free var and get the vector in the null space.
        for row, pivot in enumerate(pivot_cols):
            basis_vector[pivot] = -rref_A[row][free_var]
        null_space = np.append(basis_null_space, [round(i, 8) for i in basis_vector])
                
    # Compute for [A|0]
    if np.all(rref_A[:, -1] == 0):
        return (np.array(null_space))
    # Compute for [A|b]
    else:
        # Compute particular solution
        particular = np.zeros(cols - 1)
        for row, pivot in enumerate(pivot_cols):
            particular[pivot] = rref_A[row, -1]
        return np.array(particular), np.array(null_space)

def solve_homogeneous(A):
    """
    This function uses gaussian elimination to solve a homogenous
    system of linear equations (Ax=0).
    
    Parameters:
    - A: The coefficient matrix
    
    Returns:
    - 0: if A only has the trivial solution
    - tuple: two different non-trivial solutions
    """
    # YOUR CODE HERE
    rows, cols = A.shape
    # define the zero vector
    zero_vector = np.zeros((rows, 1))
    # Row-reduce the augmented matrix
    augmented_A = np.hstack((A, zero_vector))
    rref_A = gaussian_elim(augmented_A)
    #print(f"The matrix is:\n {A}")
    #print(f"The row-reduced form of the matrix is:\n {rref_A}")

    # find the rank of A
    rank = 0
    for row in rref_A:
        if all(element == 0 for element in row[:-1]):
            continue
        else:
            rank += 1
    # If rank = number of columns then trivial solution
    if rank == cols:
        return 0
    elif rank < cols:
        # return the non-trivial solution
        sol = linalg_solve(rref_A, rows, cols)
        return (sol, list(map(lambda x: x * 2, sol)))

def solve_nonhomogeneous(A_aug):
    """
    This function solves the non-homogenous system of linear 
    equations Ax = b

    Parameters:
    - A_aug: augmented matrix [A|b]

    Returns:
    - None: if there are no solutions
    - rtn: np array of single solution
    - (rtn, rtn): tuple of solutions if there are more than one
    """
    # TODO: solve for x for non homogeneous system
    rows, cols = A_aug.shape
    # First need to row-reduce the augmented matrix in question
    rref_A = gaussian_elim(A_aug)
    # print(f"The original matrix is:\n {A_aug}")
    # print(f"The row-reduced form of the matrix is:\n {rref_A}")
    # For system to have no solution, rk(A) != rk([A\b])
    rank_A, rank_A_aug = 0, 0
    for row in rref_A:
        if all(element == 0 for element in row):
            rank_A_aug += 1
        if all(element == 0 for element in row[:-1]):
            rank_A += 1
    if rank_A != rank_A_aug:
        # system has no solution
        return None
        
    particular, null_space_1 = linalg_solve(rref_A, rows, cols)
    # If A has full rank and sqaure
    if rank(rref_A[:, :-1]) == A_aug[:, :-1].shape[1] and A_aug[:, :-1].shape[0] == A_aug[:, :-1].shape[1]:
        # System has unique solution
        # print("Unique solution:")
        return particular
    else:
        # System has multiple solutions
        homogeneous_solutions = solve_homogeneous(A_aug[:, :-1]) 
        rtn = ()
        for arr in (particular + homogeneous_solutions):
            rtn += (arr,)
        # print(f"the tuple is:\n {rtn}")
        return rtn

def rank(A):
    # TODO: get rank of A
    rows, cols = A.shape
    rref_A = gaussian_elim(A)
    # print(rref_A)
    # rank is number of non-zero rows after rref, so loop through rows
    # to see how many rows are non-zero
    # set defualt rank to 0
    rank = 0
    for row in rref_A:
        if all(element == 0 for element in row):
            continue
        else:
            rank += 1
    return rank
    
def dim_null(A):
    # TODO: get dimension of the null space of A
    rows, cols = A.shape
    rref_A = gaussian_elim(A)
    # the nullity is the number of free variables after rref, so 
    # identify columns with pivots (leading 1)
    pivot_columns = []
    
    for row in rref_A:
        for col, element in enumerate(row):
            if element == 1:
                pivot_columns.append(col)
                break
    # n - len
    return cols - len(pivot_columns)

def basis_col(A):
    # TODO: get a basis for the column space.
    rows, cols = A.shape
    rref_A = gaussian_elim(A)
    
    # identify columns with pivots (leading 1)
    pivot_columns = []
    
    for row in rref_A:
        for col, element in enumerate(row):
            if element == 1:
                pivot_columns.append(col)
                break
    # The basis that spans the col space are the columns that
    # contain a pivot
    # Need to output the columns from pivot columns (has col idx)
    if pivot_columns == []:
        return np.zeros((rows,1))
    else:
        return A[:, pivot_columns] 

def basis_null(A):
    # TODO: get a basis for the null space.
    rows, cols = A.shape
    rref_A = gaussian_elim(A)
    
    # Identify pivot columns
    pivot_columns = [] # indexes of columns that are pivots
    for row in rref_A:
        for col, element in enumerate(row):
            if element == 1:
                pivot_columns.append(col)
                break

    # Identify free variables (columns that do not have leading ones
    # (pivots))
    free_vars = [i for i in range(cols) if i not in pivot_columns]
    
    basis_null_space = []
    # for each free var (non pivot col), form a basis vector
    for free_var in free_vars:
        basis_vector = np.zeros(cols) # fill with zeros
        basis_vector[free_var] = 1 # set free var to 1 to solve basic variables
    
        # Loop through to populate the entries using the 
        # free var and get the vector in the null space.
        for row, pivot in enumerate(pivot_columns):
            basis_vector[pivot] = -rref_A[row][free_var]

        basis_null_space.append([round(i, 6) for i in basis_vector])
    
    if basis_null_space == []:
        return np.zeros((rows,1))
    else:
        return np.array(basis_null_space).T

######################################## Testing ###########################################
# test the gaussian_elim function
def test_gaussian_elim():
    for i in range(100):
        m, n = np.random.randint(low=5, high=10, size=2)
        a = np.random.randn(m, n)
        sol1 = gaussian_elim(a)
        sol2 = np.array(sp.Matrix(a).rref()[0])
        if np.sum((sol1 - sol2) ** 2) > 1e-6:
            print(a, "\n")
            print(gaussian_elim(a), "\n")
            print(np.array(sp.Matrix(a).rref()[0]), "\n")
            return False
    test_cases = [np.array([[2, 0, 1, 1],
                            [2, 0, 1, 1],
                            [0, 0, 0, 0],
                            [1, 1, 1, 0]], dtype=np.float64),
                  np.array([[1, 2, 3],
                            [4, 5, 6],
                            [7, 8, 9]], dtype=np.float64),
                  ]
    for test in test_cases:
        sol1 = gaussian_elim(test)
        sol2 = np.array(sp.Matrix(test).rref()[0])
        if np.sum((sol1 - sol2) ** 2) > 1e-6:
            print(test, "\n")
            print(gaussian_elim(test), "\n")
            print(np.array(sp.Matrix(test).rref()[0]), "\n")
            return False
    return True
test_gaussian_elim()

def test_homogeneous_trivial():
    # test A with m>n and m=n but no dependent rows in it
    for i in range(1000):
        m = np.random.randint(low=4, high=6)
        n = np.random.randint(low=2, high=5)
        a = np.random.randn(m, n)
        res = solve_homogeneous(a)
        if res != 0:
            print(a, '\n')
            print(f' solution should be 0 but got {res}')
            return False
    # test A with m>n and there are dependent rows in it
    test_list = [
        np.array([[1, 2], [3, 4], [2, 4]]),
        np.array([[1, 2, 3], [2, 4, 6], [3, 4, 5], [4, 7, 9]]),
    ]
    for case in test_list:
        res = solve_homogeneous(case)
        if res != 0:
            print(a, '\n')
            print(f' solution should be 0 but got {res}')
            return False
    return True

def test_homogeneous_nontrivial():
    import random
    # test A with m=n and m<n
    for i in range(1000):
        m = np.random.randint(low=2, high=5)
        n = np.random.randint(low=4, high=6)
        a = np.random.randn(m, n)
        a_ = a.copy()
        # create matrices whose rows or columns are dependent
        dpdt = random.sample(range(0, m), np.random.randint(low=2, high=m + 1))
        duplicate = a[dpdt[0]]
        for row in dpdt:
            a[row] = duplicate
        x1 = np.expand_dims(solve_homogeneous(a)[0], axis=1)
        x2 = np.expand_dims(solve_homogeneous(a)[1], axis=1)
        if abs(np.sum(a @ x1)) > 1e-6 or abs(np.sum(a @ x2)) > 1e-6 or (x1==x2).all():
            print(a_, '\n')
            print('the solutions are not correct')
            return False
    # test A with m>n
    test_list = [
        np.array([[1, 2], [1, 2], [2, 4]]),
        np.array([[1, 2, 3], [3, 6, 9], [1, 2, 3], [4, 7, 9]]),
        np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
    ]
    for case in test_list:
        x1 = np.expand_dims(solve_homogeneous(case)[0], axis=1)
        x2 = np.expand_dims(solve_homogeneous(case)[1], axis=1)
        if abs(np.sum(case @ x1)) > 1e-6 or abs(np.sum(case @ x2)) > 1e-6 or (x1==x2).all():
            print(a_, '\n')
            print('the solutions are not correct')
            return False
    return True

print(test_homogeneous_trivial())
print(test_homogeneous_nontrivial())

def test_nonhomogeneous_no_solution():
    import random
    for i in range(1000):
        m = np.random.randint(low=3, high=6)
        n = np.random.randint(low=3, high=6)
        a = np.random.randn(m, n)
        a_ = a.copy()
        # create matrices whose rows conflict
        cflt = random.sample(range(0, m), np.random.randint(low=2, high=m + 1))
        duplicate = a[cflt[0]]
        for row in cflt:
            a[row] = duplicate
            a[row][-1] = np.random.normal(loc=a[row][-1])
        if solve_nonhomogeneous(a):
            print(a_, '\n')
            print(f'the solution should be None, but got {solve_nonhomogeneous(a)}')
            return False
    return True

def test_nonhomogeneous_single_solution():
    # test square A
    for i in range(1000):
        m = np.random.randint(low=3, high=6)
        n = m + 1
        a = np.random.randn(m, n)
        x = np.expand_dims(solve_nonhomogeneous(a), axis=1)
        if abs(np.sum(a[:, :-1] @ x) - np.sum(a[:, -1])) > 1e-6:
            print(a, '\n')
            print('the solution is not correct')
            return False
    test_list = [
        np.array([[1, 1, 1], [1, 3, 3], [2, 2, 2]]),
        np.array([[1, 2, 3, 2], [2, 4, 6, 4], [3, 6, 9, 6], [4, 7, 9, 10], [1, 2, 4, 7]]),
    ]
    # test A with m>n
    for case in test_list:
        x = np.expand_dims(solve_nonhomogeneous(case), axis=1)
        if abs(np.sum(case[:, :-1] @ x) - np.sum(case[:, -1])) > 1e-6:
            print(a, '\n')
            print('the solution is not correct')
            return False
    return True

def test_nonhomogeneous_infinite_solution():
    import random
    for i in range(1000):
        # test A with m=n and m<n whose rows are dependent
        m = np.random.randint(low=2, high=5)
        n = m + np.random.randint(low=1, high=4)
        a = np.random.randn(m, n)
        a_ = a.copy()
        # create matrices whose rows or columns are dependent
        dpdt = random.sample(range(0, m), np.random.randint(low=2, high=m + 1))
        duplicate = a[dpdt[0]]
        for row in dpdt:
            a[row] = duplicate
        x1 = np.expand_dims(solve_nonhomogeneous(a)[0], axis=1)
        x2 = np.expand_dims(solve_nonhomogeneous(a)[1], axis=1)
        if abs(np.sum(a[:, :-1] @ x1) - np.sum(a[:, -1])) > 1e-6 or abs(
                np.sum(a[:, :-1] @ x2) - np.sum(a[:, -1])) > 1e-6 or (x1==x2).all():
            print(a_, '\n')
            print('the solutions are not correct')
            return False
        # test A with m<n whose rows are not dependent
        if n > m + 1:
            x1 = np.expand_dims(solve_nonhomogeneous(a_)[0], axis=1)
            x2 = np.expand_dims(solve_nonhomogeneous(a_)[1], axis=1)
            if abs(np.sum(a[:, :-1] @ x1) - np.sum(a[:, -1])) > 1e-6 or abs(
                    np.sum(a[:, :-1] @ x2) - np.sum(a[:, -1])) > 1e-6 or (x1==x2).all():
                print(a_, '\n')
                print('the solutions are not correct')
                return False
    # test A with m>n whose rows are dependent
    test_list = [
        np.array([[1, 2, 3], [2, 4, 6], [3, 6, 9]]),
        np.array([[1, 2, 3, 4], [2, 4, 6, 8], [1, 4, 7, 9], [1, 2, 3, 4], [2, 4, 6, 8]]),
    ]
    for case in test_list:
        x1 = np.expand_dims(solve_nonhomogeneous(case)[0], axis=1)
        x2 = np.expand_dims(solve_nonhomogeneous(case)[1], axis=1)
        if abs(np.sum(case[:, :-1] @ x1) - np.sum(case[:, -1])) > 1e-6 or abs(
                np.sum(case[:, :-1] @ x2) - np.sum(case[:, -1])) > 1e-6 or (x1==x2).all():
            print(case, '\n')
            print('the solutions are not correct')
            return False
    return True

print(test_nonhomogeneous_no_solution())
print(test_nonhomogeneous_single_solution())
print(test_nonhomogeneous_infinite_solution())

def test_rank():
    for _ in range(1000):
        rnd_h = np.random.randint(1, 10)
        rnd_w = np.random.randint(1, 10)
        rnd_mat = np.random.choice(100, [rnd_h, rnd_w], replace=True).astype(np.float64)
        if rank(rnd_mat) != np.linalg.matrix_rank(rnd_mat):
            return False
    return True
test_rank()
