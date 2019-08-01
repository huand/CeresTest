import numpy as np
import matplotlib.pyplot as plt

xy = np.genfromtxt('XY.csv', delimiter=',')
mc = np.genfromtxt('mc.csv', delimiter=',')
m = mc[0]
c = mc[1]
data_x = xy[:, 0]
data_y = xy[:, 1]
plt.scatter(data_x, data_y)
nplot = 300
x = np.linspace(np.min(data_x), np.max(data_x), nplot)
y_est = np.exp(x * m + c)
y_gt = np.exp(x * 0.234 + 0.0543)

plt.plot(x, y_est, 'r')
plt.plot(x, y_gt, color='k')
plt.show()
