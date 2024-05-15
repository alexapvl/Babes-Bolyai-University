#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
	char* x;
	size_t len;
	int frecv[26] = {0};
	int no_chars;
	int ptoc1[2], c1toc2[2], c2top[2];
	if (pipe(ptoc1) == -1 || pipe(c1toc2) == -1 || pipe(c2top) == -1) {
		perror("error on pipe");
		exit(1);
	}
	do {
		no_chars = getline(&x, &len, stdin);
		no_chars--;
		if (x[0] == 'x' && no_chars == 1)
			break;
		write(ptoc1[1], x, no_chars * sizeof(char)); // transmit read string to child 1
		int fork1 = fork();
		if (fork1 == 0) { // child 1
			char string_read[250];
			if (read(ptoc1[0], string_read, no_chars * sizeof(char)) == -1) {
				perror("error on reading from pipe");
				exit(2);
			}
			if (string_read[no_chars - 1] == '\n')
				string_read[no_chars - 1] = '\0';
			for (int i = 0; string_read[i] != '\0'; i++) {
				frecv[string_read[i] - 'a']++;
			}
			write(c1toc2[1], frecv, 26 * sizeof(int));
			exit(0);
		}
		wait(0); // wait for child 1 to finish
		int fork2 = fork();
		if (fork2 == 0) { // child 2
			int sum = 0;
			int frecv_read[26];
			read(c1toc2[0], frecv_read, 26 * sizeof(int));
			for (int i = 0; i < 26; i++)
				sum += frecv_read[i];
			write(c2top[1], &sum, sizeof(int));
			write(c2top[1], frecv_read, 26 * sizeof(int));
			exit(0);
		}
		wait(0); // wait for child  2 to finish
		int sum_read_from_c2;
		read(c2top[0], &sum_read_from_c2, sizeof(int));
		int frecv_read_from_c2[26];
		read(c2top[0], frecv_read_from_c2, 26 * sizeof(int));
		printf("Sum of the frequencies is: %d\n", sum_read_from_c2);
		for (int i = 0; i < 26; i++) {
			char k = 'a' + i;
			printf("%c has the frequency -> %d\n", k, frecv_read_from_c2[i]);
		}
	}while (x[0] != 'x' || no_chars != 1); // read while we do not read only the character x
	
	free(x);
	// close pipes
	close(ptoc1[0]);
	close(ptoc1[1]);
	close(c1toc2[0]);
	close(c1toc2[1]);
	close(c2top[0]);
	close(c2top[1]);
	return 0;
}
