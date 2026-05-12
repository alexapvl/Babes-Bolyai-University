import math
import numpy as np
import sys

PI = math.pi

def f(x):
  return 1/(2+math.sin(x))


def integrate(fn, a, b, steps, debug, exact):
  count = 0
  table = np.zeros((steps, steps), dtype=float)
  pow_4 = 4 ** np.arange(steps, dtype=np.float64) - 1
  # trapezoidal rule
  h = (b - a)
  table[0, 0] = h * (fn(a) + fn(b)) / 2
  for j in range(1, steps):
      h /= 2
      count += 1
      # extended trapezoidal rule
      table[j, 0] = table[j - 1, 0] / 2
      table[j, 0] += h * np.sum( np.fromiter(
          (fn(a + i * h) for i in range(1, 2 ** j + 1, 2)),
          dtype=float
          )
      )
      # richardson extrapolation
      for k in range(1, j + 1):
          table[j, k] = table[j, k - 1] + \
              (table[j, k - 1] - table[j - 1, k - 1]) / pow_4[k]
  if debug:
      print(table, file=sys.stderr)
      if exact is not None:
          errors = [
              '%.2e' % i 
              for i in np.abs(table.diagonal() - exact)
            ]
          print('abs. error:', errors, file=sys.stderr)
  return table[-1, -1], count

if __name__ == "__main__":
    result, count = integrate(f, 0, PI/2, 9, True, (math.pi*math.sqrt(3))/9)
    print(result)
    print(count)
    print("Actual value:")
    print((math.pi*math.sqrt(3))/9)
