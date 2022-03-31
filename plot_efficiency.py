import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import glob

RK4 = np.array([142.744, 141.103, 142.407, 141.837, 142.631, 141.579])
RK2 = np.array([157.966, 157.681, 158.478, 158.453, 158.504, 157.664])
RK8 = np.array([180.963, 183.019, 180.999, 181.435, 180.507, 180.586])
RKCK = np.array([112.353, 112.58, 111.912, 113.029, 113.725, 113.473])

types = np.array(["RK2", "RK4", "RK8", "RKCK"])

RK4_mean = np.mean(RK4)
RK2_mean = np.mean(RK2)
RK8_mean = np.mean(RK8)
RKCK_mean = np.mean(RKCK)

means = np.array([RK2_mean, RK4_mean, RK8_mean, RKCK_mean])
errors = np.array([np.std(RK2), np.std(RK4), np.std(RK8), np.std(RKCK)])

fig = plt.figure(figsize=(15, 10), dpi=300)
#plt.grid(True, which="both", axis="both")
plt.rc('axes', axisbelow=True)
plt.errorbar(types, means, yerr=errors, ecolor="r")

plt.xlabel('Type of solver used')
plt.ylabel('Average time taken to run simulation (s)')

plt.title("Efficiency analysis")
#plt.show  #This will open the plot in a window if possible when uncommented
plt.savefig("Efficiency analysis.png")