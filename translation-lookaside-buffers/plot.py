import csv
import matplotlib.pyplot as plt

x = []
y = []

with open('data_1000.csv') as file:
    content = csv.reader(file, delimiter=',')
    for row in content:
        print(row)
        x.append(int(row[0]))
        y.append(float(row[1]))


print(type(x[0]))
print(y)
x_pos = range(len(x))

fig, ax = plt.subplots()
ax.plot(x_pos, y, linestyle='-', marker='o')

ax.set_xticks(x_pos)
ax.set_xticklabels(x)


ax.set(ylabel='time per Access (μs)', xlabel='Number Of Pages', title ='TLB Size Measurement')
ax.grid()

fig.savefig("plot.png")