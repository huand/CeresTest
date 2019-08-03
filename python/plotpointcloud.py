from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

fig = pyplot.figure()
xyz = np.genfromtxt("../data/xyz.csv")
d = np.sum(xyz ** 2, axis=1)
dmed = np.median(d)
dok = d < 4 * dmed

ax = Axes3D(fig)
print(sum(dok))
s = np.ones(sum(dok))
ax.scatter(xyz[dok, 2], xyz[dok, 0], xyz[dok, 1], s=s)
pyplot.show()
