#include <stdio.h>
#include <stdlib.h>

extern void concatenate(char *result, const char *string1, const char *string2, const char *string3);
extern int scanf(const char *format, ...);
extern int printf(const char *format, ...);
extern void exit(int status);

int main() {
    char msg1[] = "Enter the first string: ";
    char msg2[] = "Enter the second string: ";
    char msg3[] = "Enter the third string: ";
    char msg[] = "The concatenated string is: %s\n";

    char format[] = "%s";
    char str1[10], str2[10], str3[10], result[30];

    // Read first string
    printf("%s", msg1);
    scanf(format, str1);

    // Read second string
    printf("%s", msg2);
    scanf(format, str2);

    // Read third string
    printf("%s", msg3);
    scanf(format, str3);

    // Concatenate strings using the concatenate function
    concatenate(result, str1, str2, str3);

    // Print the concatenated string
    printf(msg, result);

    // Exit the program
    exit(0);
}
