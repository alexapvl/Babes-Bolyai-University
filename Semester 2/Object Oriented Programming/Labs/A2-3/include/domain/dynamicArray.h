#pragma once

#include "material.h"

typedef void* TElem;

typedef void (*destroyFunction)(TElem);

typedef struct {
    int size;
    int capacity;
    TElem* elems;
    destroyFunction df;
} DynamicArray;

/*
Dynamic Array methods
*/

DynamicArray* createDynamicArray(int capacity, destroyFunction df);
void destroyDynamicArray(DynamicArray* arr);
void resize(DynamicArray* arr);
void addElement(DynamicArray* arr, TElem elem);
void deleteElementAtIndex(DynamicArray* arr, int index);
void updateElementAtIndex(DynamicArray* arr, int index, TElem elem);
void swapElements(DynamicArray* arr, int index1, int index2);
int getSize(DynamicArray* arr);
int getCapacity(DynamicArray* arr);
TElem* getElems(DynamicArray* arr);
TElem getElementAtIndex(DynamicArray* arr, int index);
char* toStringAtIndex(DynamicArray* arr, int index);
