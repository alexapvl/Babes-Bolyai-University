# @author: Apavaloaiei Alexandru - group 911

import matplotlib.pyplot as plt
import numpy as np

# f(x) = x^2
def f(x: float) -> float:
    return x ** 2

# f'(x) = 2 * x
def df(x: float) -> float:
    return 2 * x

# For plot at (d)
def draw_arrow(plt, arr_start, arr_end):
        dx = arr_end[0] - arr_start[0]
        dy = arr_end[1] - arr_start[1]
        plt.arrow(arr_start[0], arr_start[1], dx, dy, head_width=0.1, head_length=0.25, length_includes_head=True, color='black')

def main():
    while True:
        print("a. Convex function and small η => converges to minimum of function")
        print("b. η is larger => faster convergence")
        print("c. η too large => divergence")
        print("d. Function nonconvex => stuck in local minimum")
        print("0. Exit")
        option = str(input("Select option: "))

        # Parabola
        parabola_X = range(-10,11)
        parabola_Y = [i * i for i in parabola_X]

        iterations = 100

        # Option 1: (a)
        if option == 'a':
            x: float = 10
            learning_rate: float = 0.1
            X_axis1 = []
            Y_axis1 = []
            X_axis1.append(x)
            Y_axis1.append(f(x))

            for i in range(iterations):
                x = x - learning_rate * df(x)
                X_axis1.append(x)
                Y_axis1.append(f(x))
                
            plt.title(f"(a) | Apăvăloaiei Alexandru | group 911")

            plt.xlabel(r"$x_{n+1} = x_n - ηf'(x), η>0$")
            plt.ylabel(r"f(x) = $x^2$")
            
            plt.plot(parabola_X, parabola_Y, color='r', label=r"$x^2$" )    
            plt.scatter(X_axis1, Y_axis1, color='b', label="Steps")

            plt.legend(loc='upper center', bbox_to_anchor = (0.25, 0.5, 0.5, 0.5))

            # Show plot for point (a)
            plt.grid()
            plt.show()

        # Option 2: (b)
        elif option == 'b':
            x = 10
            learning_rate = 0.3
            X_axis2 = []
            Y_axis2 = []
            X_axis2.append(x)
            Y_axis2.append(f(x))

            for i in range(iterations):
                x = x - learning_rate * df(x)
                X_axis2.append(x)
                Y_axis2.append(f(x))
                
            plt.plot(parabola_X, parabola_Y, color='r')    
            plt.scatter(X_axis2, Y_axis2, color='b')

            plt.title(f"(b) | Apăvăloaiei Alexandru | group 911")

            plt.xlabel(r"$x_{n+1} = x_n - ηf'(x), η>0$")
            plt.ylabel(r"f(x) = $x^2$")
            
            plt.plot(parabola_X, parabola_Y, color='r', label=r"$x^2$" )    
            plt.scatter(X_axis2, Y_axis2, color='b', label="Steps")

            plt.legend(loc='upper center', bbox_to_anchor = (0.25, 0.5, 0.5, 0.5))

            # Show plot for point (b)
            plt.grid()
            plt.show()

        # Option 3: (c)
        elif option == 'c':
            x = 10
            learning_rate = 1.6
            X_axis3 = []
            Y_axis3 = []
            X_axis3.append(x)
            Y_axis3.append(f(x))

            for i in range(iterations):
                x = x - learning_rate * df(x)
                X_axis3.append(x)
                Y_axis3.append(f(x))
                
            plt.plot(parabola_X, parabola_Y, color='r')    
            plt.scatter(X_axis3, Y_axis3, color='b')

            plt.title(f"(c) | Apăvăloaiei Alexandru | group 911")

            plt.xlabel(r"$x_{n+1} = x_n - ηf'(x), η>0$")
                
            plt.scatter(X_axis3, Y_axis3, color='b', label="Steps")

            plt.legend(loc='upper center', bbox_to_anchor = (0.25, 0.5, 0.5, 0.5))

            # Show plot for point (c)
            plt.grid()
            plt.show()

        # Option 4: (d)
        elif option == 'd':

            x = 10
            learning_rate = 0.5

            X_axis4 = []
            Y_axis4 = []
            X_axis4.append(x)
            Y_axis4.append(np.cos(x))

            for i in range(iterations):
                x = x - learning_rate * (-1) *np.sin(x) # (-1) * np.sin(x) is the differential of np.cos(x)
                X_axis4.append(x)
                Y_axis4.append(np.cos(x))
                
            plt.scatter(X_axis4, Y_axis4, color='b')

            plt.title(f"(d) | Apăvăloaiei Alexandru | group 911")

            plt.xlabel(r"$x_{n+1} = x_n - ηf'(x), η>0$")
            plt.ylabel(r"$cos(x)$")


            cos_X = np.linspace(0, 4*np.pi, 201)
            plt.plot(cos_X, np.cos(cos_X), color='r', label=r"$cos(x)$")
            plt.scatter(X_axis4, Y_axis4, color='b', label="Steps")

            draw_arrow(plt, [4, -1.20], [3*np.pi-0.25,-1])

            plt.legend(loc='upper center', bbox_to_anchor = (0, 0.5, 0.5, 0.5))
            plt.text(1.75, -1.27, r"Local minimum: $(3*\pi,-1)$", fontweight='bold')

            # Show plot for point (d)
            plt.grid()
            plt.show()
        elif option == '0':
            break
        else:
            print("Invalid input.")

main()