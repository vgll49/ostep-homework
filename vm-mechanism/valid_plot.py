import random
import matplotlib.pyplot as plt

# config
address_space = 1000
num_addresses = 1000
seeds = [1, 2, 3]

results = []

for L in range(0, address_space + 1, 50):
    fractions = []

    for s in seeds:
        random.seed(s)
        vas = [random.randint(0, address_space - 1) for _ in range(num_addresses)]
        valid = sum(1 for va in vas if va < L)
        fractions.append(valid / num_addresses)

    avg_fraction = sum(fractions) / len(fractions)
    results.append((L, avg_fraction))

# unpack
L_vals = [x[0] for x in results]
fractions = [x[1] for x in results]

# plot
plt.plot(L_vals, fractions)
plt.xlabel("Bounds (L)")
plt.ylabel("Fraction Valid")
plt.title("Fraction of Valid Addresses vs Bounds")
plt.show()