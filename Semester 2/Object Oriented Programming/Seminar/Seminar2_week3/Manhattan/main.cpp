#include <stdio.h>
#include "math.h"
#include "distances.h"

int main() {
    double x[2], y[2];
    x[0] = 1;
    x[1] = 8;
    y[0] = 5;
    y[1] = 3;

    printf("Manhattan distance: %f\n", ManhattanDistance(x, y)); 
    printf("Absolute value of -5: %f\n", absoluteValue(-5));

    return 0;
}