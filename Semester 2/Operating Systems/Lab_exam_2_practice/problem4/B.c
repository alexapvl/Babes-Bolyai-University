#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

int gcd(int a, int b) {
	if (b == 0)
		return a;
	else
		return gcd(b, a % b);
}

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: ./B fifo_name\n");
		exit(1);
	}
	int fd = open(argv[1], O_RDONLY);
	int a, b;
	read(fd, &a, sizeof(int));
	read(fd, &b, sizeof(int));

	int common_div = gcd(a, b);
	printf("Greatest common divisor of %d and %d is: %d\n", a, b, common_div);

	close(fd);
	unlink(argv[1]);
	return 0;
}
