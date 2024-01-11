import numpy as np
import matplotlib.pyplot as plt
from random import randint
import plotly.graph_objects as go

def f(x, y, b):
    return 0.5 * (x**2 + b * y**2)

def gradient(x, y, b):
    return np.array([x, b*y])

def sk(x: int, y: int, b: int) -> int:
    return (x ** 2 + (b ** 2) * (y ** 2)) / (x ** 2 + (b ** 3) * (y ** 2))

def gradient_descent(b, learning_rate, num_iterations=100):
    path = []
    # initialising x and y with 0 so we can enter the while loop
    x = 0
    y = 0
    while x == 0 and y == 0:
        # we make this so we ensure we will not divide by 0 when randint is 0 for x and y at the same time
        x = randint(-5, 5)
        y = randint(-5, 5)
        z = f(x, y, b)
    path.append([x, y, z])

    for _ in range(num_iterations):
        learning_rate = sk(x, y, b)
        grad = gradient(x, y, b)
        x -= learning_rate * grad[0]
        y -= learning_rate * grad[1]
        z = f(x, y, b)
        path.append([x, y, z])

    return np.array(path)

def plot_contour_gui(b, path):
    x = np.linspace(-5, 5, 100)
    y = np.linspace(-5, 5, 100)
    X, Y = np.meshgrid(x, y)
    Z = f(X, Y, b)

    fig = plt.figure(dpi=150)
    ax = fig.add_subplot(111, projection='3d')

    # Plot the 3D trajectory
    ax.plot(path[:, 0], path[:, 1], path[:, 2], marker='o', color='red', label='Trajectory', linewidth=1)

    # Plot the surface
    ax.plot_surface(X, Y, Z, cmap="viridis", alpha = 0.7)

    # Set labels and legend
    ax.set_title(f"Contour Plot for b={b} | Apăvăloaiei Alexandru, group 911")
    ax.set_xlabel("x")
    ax.set_ylabel("y")
    ax.set_zlabel("z")
    ax.legend()
    plt.savefig(f'b={b}.pdf')

    plt.show()

def plot_contour_html(b, path):
    x = np.linspace(-5, 5, 100)
    y = np.linspace(-5, 5, 100)
    X, Y = np.meshgrid(x, y)
    Z = f(X, Y, b)

    # Plotly 3D Scatter plot
    fig = go.Figure()
    fig.add_trace(go.Scatter3d(
        x=path[:, 0], y=path[:, 1], z=path[:, 2],
        mode='lines+markers', marker=dict(size=5),
        name='Trajectory'
    ))

    # Plot the surface
    fig.add_trace(go.Surface(x=X, y=Y, z=Z, colorscale="viridis", opacity=0.7))

    # Set layout
    fig.update_layout(
        title=f"Contour Plot for b={b} | Apăvăloaiei Alexandru, group 911",
        scene=dict(
            xaxis_title="x",
            yaxis_title="y",
            zaxis_title="z",
        )
    )

    # Save interactive HTML file (uncomment if you wabt to save the plots in current directory)
    # fig.write_html(f"b={b}.html")
    # Open interactive HTML file in browser
    fig.show()

def main():
    num_iterations = 100
    b_values = [1, 0.5, 0.2, 0.1]

    for b in b_values:
        path = gradient_descent(b, num_iterations)
        # call html function (uncomment if you want to use it)
        # plot_contour_html(b, path)
        # call gui function
        plot_contour_gui(b, path)

main()
