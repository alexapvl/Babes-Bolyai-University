#include "dynamicArray.h"
#include <stdlib.h>

DynamicArray* createDynamicArray(int capacity, destroyFunction op) {
    DynamicArray* arr = malloc(sizeof(DynamicArray));
    if (arr == NULL)
        return NULL;
    
    arr->size = capacity;
    arr->size = 0;
    arr->elems = malloc(sizeof(TElem) * capacity);
    if (arr->elems == NULL) {
        free(arr);
        return NULL;
    }
    arr->destroyOp = op;

    return arr;
}

void destroy(DynamicArray* arr) {
    if (arr == NULL) {
        return;
    }
    for(int i = 0; i < arr->size; i++)
        arr->destroyOp(arr->elems[i]);
    free(arr->elems);
    free(arr);
}

void resize(DynamicArray* arr) {
    if (arr == NULL)
        return;
    arr->capacity *= 2;
    TElem* aux = realloc(arr->elems, sizeof(TElem) * arr->capacity);
    if(aux == NULL)
        return;
    arr->elems = aux;
}

void addTElemToDynamicArray(DynamicArray* arr, TElem elem) {
    if (arr == NULL)
        return;
    if(arr->size == arr->capacity)
        resize(arr);
    arr->elems[arr->size++] = elem;
}