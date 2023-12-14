# Apavaloaiei Alexandru, group 911

import numpy as np
import matplotlib.pyplot as plt
from random import randint

def f(x, y, b):
    return 0.5 * (x**2 + b * y**2)

def gradient(x, y, b):
    # df_x = x
    # df_y = b * y
    return np.array([x, b*y])

def gradient_descent(b, learning_rate, num_iterations=100):
    path = []
    # this will ensure that the start point is random
    # x ∈ [-100, 100]
    # y ∈ [-100, 100]
    x = randint(-100, 100)
    y = randint(-100, 100)

    for _ in range(num_iterations):
        path.append((x, y))
        grad = gradient(x, y, b)
        x -= learning_rate * grad[0]
        y -= learning_rate * grad[1]

    return np.array(path)

def plot_contour(b, path):
    x = np.linspace(-100, 100, 1000)
    y = np.linspace(-100, 100, 1000)
    X, Y = np.meshgrid(x, y)
    Z = f(X, Y, b)

    plt.contour(X, Y, Z, levels=50, cmap="viridis")
    plt.scatter(*zip(*path), c = 'red', marker = 'o', label = "Gradient Descent Path", zorder = 10, s = 10)
    plt.title(f"Contour Plot for b={b} | Apăvăloaiei Alexandru, group 911")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.legend()
    plt.show()

def main():
    # Parameters
    learning_rate = 0.1
    num_iterations = 100

    # Values of b
    b_values = [1, 0.5, 0.2, 0.1]

    # Plot for each b value
    for b in b_values:
        path = gradient_descent(b, learning_rate, num_iterations)
        plt.figure(dpi = 150)
        plot_contour(b, path)

main()