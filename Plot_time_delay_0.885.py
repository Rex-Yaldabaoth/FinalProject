import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import glob

file = np.loadtxt("output_files/data for flowrate = 0.885000.txt", skiprows = 1)
R = file[-1,-1]
file = np.delete(file, -1, 0)
(column_length, row_length) = file.shape
t = np.array([])

periods = np.array([])
periods_temp = np.array([])

for i in range(0, column_length, 1):

    if file[i,-1] == 1:
        if file[i,0] > 10:

            t = np.append(t, file[i,0])

for i in range(0, len(t), 1):

    if(i == len(t) -1):
        break

    periods_temp = np.append(periods_temp, (t[i+1] - t[i]))

periods = np.append(periods, periods_temp)
periods2 = np.insert(periods, 0, 0)
periods = np.append(periods, 0)

fig = plt.figure(figsize=(10, 10), dpi=300)
#plt.grid(True, which="both", axis="both")
plt.rc('axes', axisbelow=True)
plt.scatter(periods2, periods, marker=',', color='black', s=2)
plt.xlabel('Tn')
plt.xlim(0.03, 0.06)
plt.ylabel('Tn+1')
plt.ylim(0.03, 0.06)
plt.title("Time delay plot")
plt.savefig("Time delay 0.885.png")

