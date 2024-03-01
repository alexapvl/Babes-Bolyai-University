// Create a C function that recieves as input 2 integer numbers and returns their sum and their product.
#include <stdio.h>

void func(int a, int b, int* sum, int* prod){
    *sum = a + b;
    *prod = a * b;
}

int main(){
    int a = 3, b = 5;
    int sum, prod;
    func(a, b, &sum, &prod);
    printf("The sum is %d and the product is %d", sum, prod);

    return 0;
}