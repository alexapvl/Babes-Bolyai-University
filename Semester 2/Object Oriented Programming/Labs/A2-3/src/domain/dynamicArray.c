#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/domain/dynamicArray.h"

DynamicArray* createDynamicArray(int capacity, destroyFunction df) {
    /*
    Creates a dynamic array
    */
    DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray));
    if(arr == NULL)
        return NULL;
    
    arr->capacity = capacity;
    arr->size = 0;

    arr->elems = (TElem*)malloc(capacity * sizeof(TElem));
    if(arr->elems == NULL)
        return NULL;

    arr->df = df;

    return arr;
}

void destroyDynamicArray(DynamicArray* arr) {
    /*
    Destroy the dynamic array by first destroying each element and then freeing the memory allocated for the array
    */
    if(arr == NULL)
        return;
    
    for (int i = 0; i < arr->size; i++)
        if(arr->elems[i] != NULL)
            arr->df(arr->elems[i]);
    free(arr->elems);
    free(arr);
}

void resize(DynamicArray* arr) {
    /*
    Resizes the dynamic array if it's full
    The capacity is doubled
    */
	if (arr == NULL)
		return;

	arr->capacity *= 2;
	TElem* aux = (TElem*)realloc(arr->elems, sizeof(TElem) * arr->capacity);
	if (aux == NULL)
		return;
	arr->elems = aux;
}

void addElement(DynamicArray* arr, TElem elem) {
    /*
    Adds an element to the dynamic array
    The array is resized if it's full
    */
    if (arr == NULL)
        return;

    if (arr->size == arr->capacity)
        resize(arr);
    
    arr->elems[arr->size++] = elem;
}

void deleteElementAtIndex(DynamicArray* arr, int index) {
    /*
    Deletes an element from the dynamic array at the specified index
    */
    if (arr == NULL)
        return;
    if (index < 0 || index >= arr->size)
        return;
    arr->df(getElementAtIndex(arr, index));
    for (int i = index; i < arr->size - 1; i++)
        arr->elems[i] = arr->elems[i + 1];
    arr->size--;
}

void updateElementAtIndex(DynamicArray* arr, int index, TElem elem) {
    /*
    Updates an element from the dynamic array at the specified index
    */
    if (arr == NULL)
        return;
    if (index < 0 || index >= arr->size)
        return;
    arr->df(arr->elems[index]);
    arr->elems[index] = elem;
}

void swapElements(DynamicArray* arr, int index1, int index2) {
    /*
    Swaps two elements from the dynamic array
    */
    TElem aux = arr->elems[index1];
    arr->elems[index1] = arr->elems[index2];
    arr->elems[index2] = aux;
}

int getSize(DynamicArray* arr) {
    /*
    Returns the size of the dynamic array
    */
	if (arr == NULL)
		return 0;
	return arr->size;
}

int getCapacity(DynamicArray* arr) {
    /*
    Returns the capacity of the dynamic array
    */
	if (arr == NULL)
		return 0;
	return arr->capacity;
}

TElem* getElems(DynamicArray* arr) {
    /*
    Returns the elements of the dynamic array
    */
    return arr->elems;
}

TElem getElementAtIndex(DynamicArray* arr, int index) {
    /*
    Returns the element from the dynamic array at the specified index
    */
    if (arr == NULL)
        return NULL;
    if (index < 0 || index >= getSize(arr))
        return NULL;
    return arr->elems[index]; 
}

char* toStringAtIndex(DynamicArray* arr, int index) {
    /*
    Returns the string representation of the element from the dynamic array at the specified index
    */
    return formatMaterialToString(getElementAtIndex(arr, index));
}