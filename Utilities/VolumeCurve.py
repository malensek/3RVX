import numpy as np
import matplotlib.pyplot as plt

curve = np.loadtxt(fname='parallels-curve.txt')

plt.ion()
plt.clf()
plt.plot(curve[:, 0], curve[:, 1])
