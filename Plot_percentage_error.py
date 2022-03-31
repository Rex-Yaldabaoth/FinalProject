import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import glob

file = np.loadtxt("output_files/data for flowrate = 1.000000.txt", skiprows = 1)
R = file[-1,-1]
file = np.delete(file, -1, 0)
(column_length, row_length) = file.shape
t = np.array([])
Merr = np.array([])

periods = np.array([])
periods_temp = np.array([])

for i in range(0, column_length, 1):

    if file[i,-1] == 1:
        if file[i,0] > 10:

            t = np.append(t, file[i,0])
            Merr = np.append(Merr,file[i,4])

fig = plt.figure(figsize=(15, 10), dpi=300)
#plt.grid(True, which="both", axis="both")
plt.rc('axes', axisbelow=True)
plt.plot(t, Merr)
plt.xlabel('Time (s)')
plt.ylabel('Percentage error in mass')

plt.title("Mass percentage error for R=1.0")
#plt.show  #This will open the plot in a window if possible when uncommented
plt.savefig("Mass percentage error 1.0.png")


