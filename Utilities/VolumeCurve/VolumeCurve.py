import numpy as np
import matplotlib.pyplot as plt

curve1 = np.loadtxt(fname='parallels-curve.txt')
curve4 = np.loadtxt(fname='4.txt')
curve16 = np.loadtxt(fname='16.txt')
curve64 = np.loadtxt(fname='64.txt')
curve256 = np.loadtxt(fname='256.txt')

plt.ion()
plt.clf()

plt.suptitle('Volume Curve Transformation', fontsize=14)
plt.xlabel('Volume Level (%)')
plt.ylabel('Output Level (dB)')
plt.plot(curve1[:, 1], lw=1.5, dashes=(10, 2), label='Default Curve')
plt.plot(curve4[:, 1], lw=1.5, label='Low (2)')
plt.plot(curve16[:, 1], lw=1.5, label='Medium (4)')
plt.plot(curve64[:, 1], lw=1.5, label='High (6)')
plt.plot(curve256[:, 1], lw=1.5, label='Custom (8)')

plt.legend(fancybox=True, shadow=True, ncol=2, loc=4)

plt.savefig('curve.pdf', bbox_inches='tight')
