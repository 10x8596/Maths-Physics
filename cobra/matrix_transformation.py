import numpy as np

# Rotation matrix for theta = pi/4
theta = np.pi / 4
R = np.array([[np.cos(theta), -np.sin(theta)],
              [np.sin(theta), np.cos(theta)]])

transposed_R = R.T

# Matrix to be transformed
M = np.array([[2, 1],
              [1, 3]])

M_transformed = np.dot(transposed_R, M)
print(M_transformed)

final_M = np.dot(M_transformed, R)
print(final_M)
