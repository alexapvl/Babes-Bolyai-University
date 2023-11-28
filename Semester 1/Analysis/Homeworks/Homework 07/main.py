import numpy as np

x = np.linspace(-5, 5, 1000)
# y = f(x) = e^(-x)
y = np.exp(-x ** 2)

sum = np.sum((x[1:] - x[:-1]) / 2 * (y[1:] + y[:-1]))

print(f"My result is: {sum}")
print(f"Square root of pi is {np.sqrt(np.pi)}")