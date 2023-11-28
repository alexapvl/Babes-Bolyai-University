# Apavaloaiei Alexandru, group 911

import numpy as np
import matplotlib.pyplot as plt   

points = np.random.uniform(-1, 1, size=(3000,2))

p = float(input("Enter p: "))
plt.figure(dpi=100)

for point in points:
    if np.float_power(np.float_power(np.abs(point[0]), p) + np.float_power(np.abs(point[1]), p), 1/p) <= 1:
        plt.scatter(point[0], point[1], color="red", s=2)

plt.title(f"p = {p} | Apăvăloaiei Alexandru | group 911")
plt.xlabel("x - axis")
plt.ylabel("y - axis")
plt.grid()
plt.show()
