import numpy as np

x = np.linspace(-5, 5, 1000)
# f(x) = e^(-x)
y = np.exp(-x ** 2)

np.sum(y[1:-1])

    


# print(f"My result is: {format(np.sum(f))}")
print(f"Square root of pi is {np.sqrt(np.pi)}")