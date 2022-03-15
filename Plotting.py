import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import glob



files = list(glob.glob("output_files/*.txt"))
periods = np.array([])
flows = np.array([])
iteration = 0

for file_name in files:
    iteration += 1
    print(file_name, iteration)
    try:
        current_file = np.loadtxt(file_name, skiprows = 1)
    except:
        print("Couldn't load file")
        continue
    periods_temp = np.array([])
    R = current_file[-1,-1]
    current_file = np.delete(current_file, -1, 0)
    
    (column_length, row_length) = current_file.shape
    
    t = np.array([])

    for i in range(0, column_length, 1):

        if current_file[i,4] == 1:

            t = np.append(t, current_file[i,0])
    
    for i in range(0, len(t), 1):

        if(i == len(t) -1):
            break

        periods_temp = np.append(periods_temp, (t[i+1] - t[i]))
    
    flows = np.append(flows, np.full(periods_temp.shape, R))
    periods = np.append(periods, periods_temp)

fig = plt.figure(figsize=(5, 5), dpi=250)
plt.grid(True, which="both", axis="both")
plt.rc('axes', axisbelow=True)
plt.scatter(flows, periods, marker='.')
plt.xlabel('Flow rate (R)')
plt.ylabel('Time period between drips (s)')
plt.title("Bifrucation Diagram")
plt.savefig("Testfig.png")




    
    
    

    

    


