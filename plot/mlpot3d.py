import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D

fig = plt.figure()
ax = Axes3D(fig)
P = np.arange(1.0, 224, 1.0)
R = np.arange(1.0, 100, 1.0)
P,R = np.meshgrid(P, R)
Z = (P+R-1)/(P*R)
ax.plot_surface(P,R,Z,rstride=1,cstride=1,cmap='rainbow')

plt.show()
