import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 unused import

n = 200
theta = np.random.rand(n, 1) * 2 * np.pi
r = 20
x = r * np.cos(theta) + np.random.rand(n)*r/4
y = r * np.sin(theta) + np.random.rand(n) * r / 4
z = np.zeros(n)

xyz = r*np.concatenate((np.cos(theta), np.sin(theta),
                        np.zeros((n, 1))), axis=1)
xyz += np.random.rand(n, 3) * r / 3
xyz1 = np.concatenate((xyz, np.ones((n, 1))), axis=1)
theta = np.random.rand() * 2 * np.pi
phi = np.random.rand() * 2 * np.pi

Rtheta = np.array(((np.cos(theta), 0, np.sin(theta)),
                   (0, 1, 0), (-np.sin(theta), 0, np.cos(theta))))
Rphi = np.array(
    ((1, 0, 0), (0, np.cos(phi), np.sin(phi)), (0, -np.sin(phi), np.cos(phi))))
R = Rtheta.dot(Rphi)
t = np.random.rand(3, 1) * r
np.concatenate((t, np.zeros((1, 1))))
T01 = np.concatenate(
    (np.concatenate((R, np.zeros((1, 3)))), np.concatenate((t, np.ones((1, 1))))), axis=1)
P = T01.dot(np.transpose(xyz1))
P = P[0:3, :]
P = np.transpose(P)
print(P)
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(P[:, 0], P[:, 1], P[:, 2])
np.savetxt("circle_noisy.csv", P, delimiter=" ")
plt.show()
