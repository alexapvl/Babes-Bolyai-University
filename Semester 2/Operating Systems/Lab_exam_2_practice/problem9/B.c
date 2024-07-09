#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char** argv) {
	int fd = open("p2toB", O_RDONLY);
	int v[5];
	read(fd, v, (6 * sizeof(int)));
	
	printf("A -> %d\n", v[0]);
	printf("E -> %d\n", v[1]);
	printf("I -> %d\n", v[2]);
	printf("O -> %d\n", v[3]);
	printf("U -> %d\n", v[4]);

	unlink("p2toB");
	close(fd);
	return 0;
}
