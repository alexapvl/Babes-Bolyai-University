#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int count_vowels(char* word) {
	int result = 0;
	char vowels[] = "aeiouAEIOU";
	for (int i = 0; word[i] != '\0'; i++) {
		for (int j = 0; j < 10; j++) { // number of possible vowels
			if (word[i] == vowels[j])
				result++;
		}
	}
	return result;
}

int main(int argc, char** argv) {
	if (argc < 3 || argc % 2 != 1) {
		printf("Invalid number of arguments\n Usage is: ./%s string1 number1 string2 number2 ...\n", argv[0]);
	}

	for (int i = 1; i <= argc; i+=2) {
		pid_t pid = fork();
		if (pid == 0) {
			int expected_vowels = atoi(argv[i + 1]);
			int number_vowels = count_vowels(argv[i]);
			if (expected_vowels == number_vowels) {
				printf("%s %s\n", argv[i], argv[i + 1]);
			}
			exit(0);
		}

		if(pid < 0) {
			perror("error on fork\n");
			return 1;
		}
	}

	for (int i = 0; i < (argc - 1) / 2; i++)
		wait(0); // wait for all child processes to finish

	return 0;
}
