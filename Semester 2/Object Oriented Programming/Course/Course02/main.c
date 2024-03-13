#include <stdio.h>
#include <stdlib.h>

typedef int TElem;

typedef struct{
    TElem* elems;
    int size, capacity;

}DynamicArray;

DynamicArray* createArray(int capacity){
    DynamicArray* da = malloc(sizeof(DynamicArray));
    if(da == NULL)
        return da;
    
    da->capacity = capacity; // same as (*da).elems
    da->size = 0; // same as (*da).size
    // because da is a pointer to that structure, so we need to dereference it before using it
    da->elems = malloc(capacity * sizeof(TElem));

    return da;
}

void destroyDynamicArray(DynamicArray* da){
    if(da == NULL)
        return;
    
    free(da->elems); // free the elements which are in memory from the structure "da"
    free(da); // free the structure allocated in memory
}

typedef struct{
    double arr[100];
    int size;
}Vector;

typedef double(*operation)(double, double); // defining a function pointer

double add(double a, double b){
    return a + b;
}

double multiply(double a, double b){
    return a * b;
}

double applyOperation(Vector v, operation op){
    double result = v.arr[0];
    for(int i =1; i < v.size; i++)
        result = op(result, v.arr[i]);
    
    return result;
}

int main(){
    DynamicArray* da = createArray(10);
    // we will have memory leaks if we do not destroy the array
    destroyDynamicArray(da);

    // -----------------------------------

    // int* p = malloc(sizeof(int)); // dynamically allocating memory using the heap
    // if(p != NULL) // good practice
    //     *p = 10;

    // // we are going to leak memory when we do not free the allocated memory on the heap
    // // When "I" do the allocation, I should also do the deallocation
    // free(p);

    // -----------------------------------

    // char operationType;
    // double result = 0;
    // Vector v;
    // v.size = 3;
    // v.arr[0] = 2;
    // v.arr[1] = 6;
    // v.arr[2] = 10;
    // printf("Input an operation: ");
    // scanf("%c", &operationType);
    // if (operationType == '+')
    //     result = applyOperation(v, &add);
    // else
    //     result = applyOperation(v, &multiply);

    // printf("The result is %lf\n", result);

    // -----------------------------------

    // int x = 5;
    // int* px = &x; // at the address indicated by px we can find the exact value of x
    // int** ppx = &px; // at the address indicated by ppx we can find the exact value of px

    // int* p = &p; // you can do this :)
    // *p = 100; // I believe this changes the value of the pointer p to 100 so then the pointer will point to address 100(decimal) in memory


    // // Arrays
    // int arr[10];
    // // OBS: the name of the array is a pointer to the first element of the array
    // int* parr1 = arr;
    // int* parr2 = &arr[0]; // these two values should be the same

    // // the elements in memory are represented in a contiguous block, one after the other

    // // int elem0 = arr[0];
    // // elem0 = *arr; // *arr == arr[0]
    // // int elem1 = arr[1];
    // // elem1 = *(arr + 1); // arr[1] == *(arr + 1) == 1[arr]
    // // elem1 = 1[arr];

    // // OBS: when having an array as an argument in a function, you don't need to dereference(*)
    // // it because the name of the array itself is aleready a pointer
    return 0;
}