
from matplotlib import pyplot
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

fig = pyplot.figure()
ax = Axes3D(fig)
xyz = np.genfromtxt("xyz.csv")
ax = Axes3D(fig)
ax.scatter(xyz[1:5000, 0], xyz[1:5000, 1], xyz[1:5000, 2])
pyplot.show()
