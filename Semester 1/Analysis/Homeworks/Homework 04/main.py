import matplotlib.pyplot as plt
import numpy as np

# The range of element on the x-axis
sizeX = int(input("Enter the range of x on the Ox axis: "))
xlist = np.arange(1, sizeX, 1)

# The generation of the y-axis, based on the size of the x-axis
ylist = np.empty(xlist.size)

# Initialisation of the first element of y in order to be able to use a recurrent formula to determine all the other elements
ylist[0] = 1
for i in range(1, xlist.size):
    ylist[i] = ylist[i-1] + ((-1) ** (xlist[i]+1) / xlist[i])

"""
We will generate a positive and a negative list of the elements from the series in order for us to be able to 
increment the indexes of the positive and negative lists at the same rate as the elements from the zlist 
(rate = 1). In order to do this we will need the sizes of the positive and negative lists to be the same as "xlist".
This will cover every case for the values p and q, so we are not left without positive elements or negative ones when 
generating "zlist".
"""

# We initialize the lists for positive and negative numbers that will have the same number of elements as "xlist"
positiveList = np.empty(xlist.size)
negativeList = np.empty(xlist.size)

# The generation of the positive elements from the series
index: int = 0
while index < positiveList.size:
    positiveList[index] = 1 / (index * 2 + 1)
    index += 1
    
# We reset the value in order to generate the negative elements list from the series
index = 0
while index < negativeList.size:
    negativeList[index] = (-1) / ((index + 1) * 2)
    index += 1
    
# Intigers to keep count of the index of positive/negative values from "xlist"
positive_index: int = 0
negative_index: int = 0

zlist = np.empty(xlist.size)

"""
    p and q are declared in order to be able to change their value to explore with the convergence of the given series
One can even take p and q as input if they would like, in our case we will explore the following case in which p and q
are equal to 1 and 2 respectively because we know that then the sum will converge to ln(2)/2 from the course.
    Note that we can change the code to take p and q as input and compare other modified sums.
"""
  
p: int = 1 # "p" positive numbers
q: int = 2 # Folowed by "q" negative and so on...

for index_zlist in range(0, zlist.size):
    if index_zlist % (p+q) < p:
        zlist[index_zlist] = zlist[index_zlist-1] + positiveList[positive_index] # The recurrent formula based on p and q
        positive_index += 1
    else: # if index_zlist % (p+q) >= p
        zlist[index_zlist] = zlist[index_zlist-1] + negativeList[negative_index] # The recurrent formula based on p and q
        negative_index += 1

# Creates the figure with a desired pixel density for image quality
plt.figure(dpi = 150)

# Based on the x-axis and y-axis, the sum is represented and labeled
plt.plot(xlist, ylist, label  = r'$\sum_{n=1}^\infty \frac{(-1)^{n+1}}{n}$')

plt.plot(xlist, zlist, label = f"The modified sum")

# Creates a horizontal line that has the value y = ln(2)
lnlist = plt.axhline(y = np.log(2), linestyle = '-', label = r'$\ln{2}$', color = 'r')

# Creates a horizontal line that has the value y = (ln(2))/2 -> the new limit when we take 1 positive term and the next 2 negative ones 
halflnlist = plt.axhline(y = np.log(2)/2, linestyle = '-', label = r'$\frac{\ln{2}}{2}$', color = 'g')

# Generates a grid for the plot for convenience of the user
plt.grid()

# Gives a title to the plot
plt.title(f"n = {sizeX} | Apăvăloaiei Alexandru | group 911")

# Names the x and y axis
plt.xlabel("x-axis")
plt.ylabel("y-axis")

# Shows legend of labels that were assigned and anchor the legend in such a place that it will not disturb the view of the user
plt.legend(loc='upper center', bbox_to_anchor = (0.5, 0.5, 0.5, 0.5), mode = 'expand')
plt.show()