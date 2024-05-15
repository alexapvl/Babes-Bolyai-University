#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/uio.h>

int main(int argc, char** argv) {
	if (argc != 4) {
		printf("Usage: ./A fifo_name number1 number2\n");
		exit(1);
	}

	mkfifo(argv[1], 0600);
	int fd = open("myfifo", O_WRONLY);

	int fork1 = fork();
	if (fork1 == 0) { // proc 1
		int a, b, sum;
		a = atoi(argv[2]);
		b = atoi(argv[3]);
		sum = a + b;
		write(fd, &sum, sizeof(int));
	}

	int fork2 = fork();
	if (fork2 == 0) { // proc 2
		int a, b, prod;
		a = atoi(argv[2]);
		b = atoi(argv[3]);
		prod = a * b;
		write(fd, &prod, sizeof(int));
	}

	wait(0);
	wait(0);
	close(fd);
	return 0;
}
