#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: ./%s name_of_fifo\n", argv[1]);
		exit(1);
	}
	int p[2];
	if (pipe(p) == -1) {
		perror("error on making the pipe");
		exit(2);
	}
	int fork1 = fork();
	if (fork1 == -1) {
		perror("error on fork1");
		exit(3);
	}
	if (fork1 == 0) { // P1
		close(p[0]); // close the read end from the pipe
		int fifofd = open(argv[1], O_RDONLY);
		char digit;
		int sum = 0;
		while (read(fifofd, &digit, sizeof(char)) != 0) {
			sum += (digit - '0');
		}
		if (write(p[1], &sum, sizeof(int)) == -1) {
			perror("error on the transmision of the sum to the second process");
			exit(4);
		}
		close(p[1]);
		close(fifofd);
		exit(0);
	}

	int fork2 = fork();
	if (fork2 == -1) {
		perror("error on fork2");
		exit(5);
	}
	if (fork2 == 0) { // P2
		close(p[1]); // close the write end of the pipe
		int sum;
		if (read(p[0], &sum, sizeof(int)) == -1) {
			perror("error on reading the sum from the pipe");
			exit(7);
		}
		for(int i = 1; i * i <= sum; i++) {
			if (sum % i == 0) {
				if (i * i == sum)
					printf("%d is a root of %d\n", i, sum);
				else
					printf("%d, %d\n", i, sum / i);
			}
		}
		close(p[0]);
		exit(0);
	}

	close(p[0]);
	close(p[1]);
	wait(0);
	wait(0);
	
	if (unlink(argv[1]) == -1) {
		perror("error on unlinking fifo");
		exit(5);
	}
	return 0;
}
