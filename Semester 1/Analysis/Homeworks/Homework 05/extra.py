# @author: Apavaloaiei Alexandru - group 911

import matplotlib.pyplot as plt
import numpy as np

X = np.arange(-2, 2, 0.007)
# OBS: if overflow, change the third 

def weierstrass(X: list, N: int, a: float, b: int) -> list:
    """
    Args:
        X (list): The Ox axis
        N (int): The upper bound of the indexes of the sum (it will be pretty high since we want to have
        an accurate result)
        a (float): From wikipedia, 0 < a < 1
        b (float): From wikipedia, b is a positive odd intiger
        From wikipedia, a * b > 1 + 3/2 * pi

    Returns:
        list: Y -> Weierstrass function
    """
    Y = np.zeros(X.size)
    for n in range(0, X.size):
        Y = Y + a ** n * np.cos(b ** n * np.pi * X)
    return Y

plt.figure(dpi=150)
plt.title(f"Weierstrass Function | Apăvăloaiei Alexandru | group 911")
plt.plot(X, np.reshape(weierstrass(X, 500, 0.5, 3), X.size))
plt.grid()
plt.show()