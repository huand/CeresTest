import numpy as np
import matplotlib.pyplot as plt

sigma = 0.2
xmin = -2
xmax = 10
n = 100
m = 0.234
c = 0.0543
X = np.random.rand(n, 1) * (xmax - xmin) + xmin
Y = np.exp(m * X + c)+np.random.randn(n, 1)*sigma
XY = np.concatenate((X, Y), 1)
np.savetxt("XY.csv", XY, delimiter=",")
plt.scatter(X, Y)
plt.show()
