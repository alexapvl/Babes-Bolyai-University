#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void fibonacci(int n, int* output) {
	int a = 0, b = 1;
	output[0] = a;
	output[1] = b;
	for (int i = 2; i < n; i++) {
		output[i] = output[i - 2] + output[i - 1];
	}
}

int main(int argc, char** argv) {
	int p2c[2];
	int c2p[2];
	if (pipe(p2c) == -1) {
		perror("Error on p2c pipe");
		return 1;
	}
	if (pipe(c2p) == -1) {
		perror("Error on c2p pipe");
		return 1;
	}

	pid_t pid = fork();

	if (pid == 0) { // child
		close(p2c[1]); // close the write end for the parent -> child
		close(c2p[0]); // close the read end for the child -> parent

		int num;
		while (read(p2c[0], &num, sizeof(num)) > 0 && num > 0) {
			int output[num];
			fibonacci(num, output);
			if (write(c2p[1], output, num * sizeof(int)) == -1) {
				perror("Error on write in child process");
				exit(1);
			}
		}
	close(p2c[0]);
	close(c2p[1]);
	exit(0);

	} else { // parent
		close(p2c[0]); // close the read end for parent -> child
		close(c2p[1]); // close the write end for child -> parent
		
		int num;
		while(scanf("%d", &num) == 1 && num > 0) {
			if (write(p2c[1], &num, sizeof(num)) == -1) {
				perror("Error on write");
				return 1;
			}
			int output[num];
			if (read(c2p[0], &output, num * sizeof(int)) == -1) {
				perror("Error on read in parent process");
				exit(1);
			}
			printf("Fibonacci sequence for %d is: ", num);
			for (int i = 0; i < num; i++)
				printf("%d ", output[i]);
			printf("\n");
		}
		close(p2c[1]);
		close(c2p[0]);
		wait(0);
	}
	
	return 0;
}
