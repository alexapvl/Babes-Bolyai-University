#include <stdio.h>
// Problem 6

int read_vector(int* vector);
int leading_zeros(int* vector, int n);
int isPrime(int x);
int longestSubsequence(int* vector, int n, int* start, int* end);

int main() {
    int vector[100] = {0};
    int n, option;

    while (1) {
        // Print menu
        printf("1. Read a vector\n");
        printf("2. Show number of 0 digits of the product of the numbers from the vector\n");
        printf("3. Longest subsequence such that the sum of any two consecutive elements is a prime number\n");
        printf("4. Exit\n");
        printf(">> ");
        
        if (scanf("%d", &option) != 1) {
            // Handle non-integer input
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // If a character is inputted instead of a number, request input again from user
            continue; // Restart the loop
        }

        switch (option) {
            case 1: {
                printf("Enter the numbers:\n");
                n = read_vector(vector);
                break;
            }
            case 2: {
                printf("Number of 0 digits of the product is %d\n", leading_zeros(vector, n));
                break;
            }
            case 3: {
                int maxSubsequence[100];
                int start, end;
                int maxLength = longestSubsequence(vector, n, &start, &end);
                printf("The subsequence is:\n");
                for (int i = start; i < end; i++) {
                    printf("%d ", vector[i]);
                }
                printf("\nAnd the length of it is %d\n", maxLength);
                break;
            }
            case 4:
                return 0;
            default:
                printf("Invalid option. Please enter a valid option.\n");
                break;
        }
    }
    return 0;
}

int read_vector(int* vector) {
    /*
    Reads a vector until the number 0 is read.
    */
    int i = 0;
    int x;
    while (1) {
        scanf("%d", &x); // read a number
        if (x == 0)
            return i;
        *(vector + i) = x;
        i++;
    }
}

int leading_zeros(int* vector, int n){
    /*
    Calculates the number of zeros at the end of the product of the numbers from a given vector.
    */
    long long product = 1;
    for(int i = 0; i < n; i++){
        product *= vector[i];
    }
    int result = 0;
    while(product % 10 == 0){
        result++;
        product /= 10;
    }

    return result;
}

int isPrime(int x){
    /*
    Checks if a number is prime or not, returns 1 if prime, 0 otherwise
    */
    if(x <= 1)
        return 0;
    if(x == 2)
        return 1;
    if(x % 2 == 0)
        return 0;
    for(int d = 2; d * d <= x; d ++)
        if(x % d == 0)
            return 0;
    
    return 1;
}

int longestSubsequence(int* vector, int n, int* start, int* end){
    /*
    Checks for the longest sequence in the vector where the sum of any two consecutive elements is a prime number
    The length of the subsequence is returned by the function while the subsequence itself is represented
    by the pointers 'start' and 'end' which refer to the initial vector.
    */
    int maxLength = 0;
    for (int i = 0; i < n; i++) {
        int currentLength = 1;
        for (int j = i + 1; j < n; j++) {
            // Check if the sum of two consecutive elements is prime
            if(isPrime(vector[j - 1] + vector[j])) {
                currentLength++;
            } else {
                break;  // Break the loop if the condition is not met
            }
        }

        // Update the maximum length and starting/ending indices if needed
        if(currentLength > maxLength) {
            maxLength = currentLength;
            *start = i;
            *end = i + maxLength;
        }
    }
    return maxLength;
}
