import numpy as np
from scipy.integrate import quad

#################### Inner product of vector functions #################
# Define the functions p(x) and q(x)
def p(x):
    return 3*x**2 - 1

def q(x):
    return 2*x + 1

# Define the inner product function
def inner_product(f, g):
    return quad(lambda x: f(x) * g(x), 0, 1)[0]

# Compute the inner product of p and q
inner_prod_pq = inner_product(p, q)

print(inner_prod_pq)

# Check if the inner product is close to zero (indicating orthogonality)
tolerance = 1e-10
if np.abs(inner_prod_pq) < tolerance:
    print("The functions p(x) and q(x) are approximately orthogonal.")
else:
    print("The functions p(x) and q(x) are not orthogonal.")
