#pragma once

#include "planet.h"

typedef void* TElem;

typedef void (*destroyFunction)(TElem); // pointer to a function which recieves a TElem and returns nothing

typedef struct {
    int size;
    int capacity;
    TElem* elems;
    destroyFunction destroyOp;

} DynamicArray;

DynamicArray* createDynamicArray(int capacity, destroyFunction op);
void destroy(DynamicArray* arr);
void addTElemToDynamicArray(DynamicArray* arr, TElem elem);