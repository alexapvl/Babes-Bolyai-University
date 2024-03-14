#include "planet.h"
#include "dynamicArray.h" 

int main() {
    DynamicArray* arr = createDynamicArray(2, &destroy); // the function to the pointer is the destroy function for the elements in arr->elems
    Planet p = createPlanet("Earth", "terrestrial", 1.0);
    printf("Planet name: %s\n", getName(p));
    Planet p2 = createPlanet("Mars", "terrestrial", 1.5);
    printf("Planet name: %s\n", getName(p2));

    destroyPlanet(p);

    return 0;
}