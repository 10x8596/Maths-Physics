import autograd.numpy as np 
from autograd import grad

def f(X, M, U, V):
    X_minus_M = X - M 
    inner_term = np.dot(np.dot(X_minus_M.T, np.linalg.inv(U)), X_minus_M)
    A = np.dot(inner_term, V)
    return np.exp(-0.5 * np.trace(A))

grad_f = grad(f)

n = 2
m = 3
X = np.random.rand(n, m)
M = np.random.rand(n, m)
U = np.dot(np.random.rand(n, n), np.random.rand(n, n).T) + np.eye(n) * 1e-3
V = np.dot(np.random.rand(m, m), np.random.rand(m, m).T) + np.eye(m) * 1e-3

gradient = grad_f(X, M, U, V)
print(gradient)
