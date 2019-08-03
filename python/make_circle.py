import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 unused import

# random pure flat circle with constant radius
n = 1000
r = 20
theta = np.random.rand(n, 1) * 2 * np.pi
theta0 = np.arange(n) / n * 2 * np.pi
theta0 = np.reshape(theta0, (n, 1))
data = r*np.concatenate((np.cos(theta), np.sin(theta),
                         np.zeros((n, 1))), axis=1)
circle = r*np.concatenate((np.cos(theta0), np.sin(theta0),
                           np.zeros((n, 1))), axis=1)
# adding noise
noise_level = r
data += np.random.randn(n, 3) * noise_level

# solid 3d transformation
theta = np.random.rand() * 2 * np.pi
phi = np.random.rand() * 2 * np.pi
Rtheta = np.array(((np.cos(theta), 0, np.sin(theta)),
                   (0, 1, 0), (-np.sin(theta), 0, np.cos(theta))))
Rphi = np.array(
    ((1, 0, 0), (0, np.cos(phi), np.sin(phi)), (0, -np.sin(phi), np.cos(phi))))
R = Rtheta.dot(Rphi)
t = np.random.rand(3, 1) * r
T01 = np.concatenate(
    (np.concatenate((R, np.zeros((1, 3)))), np.concatenate((t, np.ones((1, 1))))), axis=1)
circle = np.concatenate((circle, np.ones((n, 1))), axis=1)
circle = T01.dot(np.transpose(circle))
circle = np.transpose(circle[0:3, :])
data = np.concatenate((data, np.ones((n, 1))), axis=1)
data = T01.dot(np.transpose(data))
data = np.transpose(data[0:3, :])
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.axis = 'equal'
size = np.ones(n)
ax.scatter(data[:, 0], data[:, 1], data[:, 2], s=size)
ax.plot(circle[:, 0], circle[:, 1], circle[:, 2], 'r')
np.savetxt("../data/circle_noisy.csv", data, delimiter=" ")
np.savetxt("../data/circle_pure.csv", circle, delimiter=" ")
ax.axis = 'equal'
plt.show()
