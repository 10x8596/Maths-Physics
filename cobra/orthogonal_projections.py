import numpy as np

# Defining the vectors
u1 = np.array([-1, 1, 1])
u2 = np.array([2, -1, -2])
x = np.array([8, 4, 16])

# Calculating the orthogonal projection of x onto U using the formula
proj_u1 = (np.dot(x, u1) / np.dot(u1, u1)) * u1
proj_u2 = (np.dot(x, u2) / np.dot(u2, u2)) * u2

# Sum of the two projections gives the overall projection of x onto U
projection_onto_U = proj_u1 + proj_u2
print(projection_onto_U)