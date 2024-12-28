import numpy as np

# Rotation matrix for theta = pi/4
theta = np.pi / 4
R = np.array([[np.cos(theta), -np.sin(theta)],
              [np.sin(theta), np.cos(theta)]])

# Define vectors u and v
u = np.array([1, 1])
v = np.array([2, -1])

# Define matrix D
D = np.array([[2, 1],
              [1, 3]])

D_prime = np.array([[3.5, 0.5],
                    [0.5, 1.5]])

# Compute inner products and norms
inner_product_uv = np.dot(np.dot(R, u).T, np.dot(D_prime, np.dot(R, v)))
norm_u = np.sqrt(np.dot(np.dot(R, u).T, np.dot(D_prime, np.dot(R, u))))
norm_v = np.sqrt(np.dot(np.dot(R, v).T, np.dot(D_prime, np.dot(R, v))))

# Compute cosine of the angle between u and v
cos_theta = inner_product_uv / (norm_u * norm_v)

# Compute the angle in radians
theta = np.arccos(cos_theta)

# Convert the angle to degrees
theta_degrees = np.degrees(theta)

print(np.dot(R, v))
print(f"The inner product of u and v is {inner_product_uv}")
print(f"The norm of u is {norm_u}")
print(f"The norm of v is {norm_v}")
print(np.sqrt(norm_u * norm_v))
print(f"The angle between u and v under the inner product defined by D is {theta_degrees} degrees.")
