# Author: Apăvăloaiei Alexandru group 911

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import FancyArrow
from mpl_toolkits.mplot3d import Axes3D

# Quadratic function f(x) = 1/2 * x^T * A * x
def f(x, A):
    return 0.5 * np.dot(x.T, np.dot(A, x))

# Gradient of the quadratic function
def gradient(x, A):
    return np.dot(A, x)

def main():
    # Matrix A for a unique minimum
    # A = np.array([[2, 0], [0, 3]])

    # Matrix A for a unique maximum
    # A = np.array([[-2, 0], [0, -3]])

    # Matrix A for a saddle point
    # A = np.array([[2, 0], [0, -3]])
    matrices = [np.array([[2, 0], [0, 3]]), np.array([[-2, 0], [0, -3]]), np.array([[2, 0], [0, -3]])]
    titles = ["Apăvăloaiei Alexandru -  group 911 | Unique Minimum",
              "Apăvăloaiei Alexandru -  group 911 | Unique Maximum",
              "Apăvăloaiei Alexandru -  group 911 | Saddle Point"]
    
    for index in range(3):
        A = matrices[index]
        # Generate points for plotting
        x_vals = np.linspace(-5, 5, 100)
        y_vals = np.linspace(-5, 5, 100)
        X, Y = np.meshgrid(x_vals, y_vals)
        Z = np.array([[f(np.array([x, y]), A) for x in x_vals] for y in y_vals])
        
        # Plotting
        fig = plt.figure(figsize=(12, 8))
        ax = fig.add_subplot(111, projection="3d")
        
        # Plot 3D surface
        ax.plot_surface(X, Y, Z, cmap="magma", alpha=0.7, label="Surface")

        # Plot contour lines
        contour = ax.contour(X, Y, Z, levels=10, cmap="magma", offset=np.min(Z), linewidths=2, alpha=0.7, label="Contour Lines")
        ax.clabel(contour, inline=True, fontsize=8)

        # Plot gradients at three different points (blue, brown, red)
        # Uses quiver to plot the gradient vectors which are perpendicular to the contour lines
        points = np.array([[2, 2], [-3, 3], [4, -4]])
        colors = ["blue", "green", "purple"]
        for i in range(3):
            gradient_vector = gradient(points[i], A)
            ax.quiver(points[i][0], points[i][1], f(points[i], A), gradient_vector[0], gradient_vector[1], -3,
                    color=colors[i], arrow_length_ratio=0.05, label=f"Gradient at {points[i][0], points[i][1]}")
        
        # Plot settings
        ax.set_xlabel("X")
        ax.set_ylabel("Y")
        ax.set_zlabel("f(X, Y)")
        ax.set_title(titles[index])
        ax.legend()
        plt.show()

main()
