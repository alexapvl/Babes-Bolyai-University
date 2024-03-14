#include "math.h"

double ManhattanDistance(double x[2], double y[2]) {
    double dist = 0;
    for (int i = 0; i < 2; i++) {
        dist += absoluteValue(x[i] - y[i]);
    }
    
    return dist;
}