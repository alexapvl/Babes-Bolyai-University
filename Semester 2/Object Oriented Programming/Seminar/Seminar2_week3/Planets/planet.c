#include <stdlib.h>
#include <string.h>
#include "planet.h"

Planet createPlanet(char name[], char type[], double distance) {
    Planet p;
    p.name = malloc(sizeof(char) * (strlen(name) + 1));
    p.type = malloc(sizeof(char) * (strlen(type) + 1));
    strcpy(p.name, name);
    strcpy(p.type, type);
    p.distance = distance;
    return p;
}

void destroyPlanet(Planet p) {
    free(p.name);
    free(p.type);
}

char* getName(Planet p) {
    return p.name;
}