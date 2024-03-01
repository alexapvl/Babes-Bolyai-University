#include <stdio.h>

// void increase(int x){ // pass by value
//     x += 1;
// }
void increase(int* px){ // pass by reference
    *px += 1;
    // the first * in the argument is for saying that we will have a pointer to an int (not dereferencing!!!!)
    // the second * is for dereferencing, we go to the address of the pointer that was passed in the function argument and increase the value
    // at that address, which will be the address of x
}

int main()
{
    // printf("Hello world!\n");
    // int a = 23;
    // int b = 12;
    // printf("Value of a and b are: %d %d\n", a, b);

    // int c = 0;
    // printf("Address of c is : %p", &c);

    // int x = 10;
    // int* px = &x;
    // increase(px);
    // printf("x is %d", x);

    // printf("Input c: ");
    // scanf("%d", &c);
    // printf("Value of c is %d\n", c);

    // printf("\n\n");

    // char s='a';
    // char* ps = &s;
    // printf("%c", *ps);

    char name[1];
    printf("Give me a name: ");
    scanf("%s", name);
    printf("The name is %s\n", name);

    printf("%p\n", name);
    printf("%p\n", &name[0]);
    printf("%p\n", &name[1]);

    printf("%c\n", *name);
    printf("%c\n", *(name+1));


    return 0;
}