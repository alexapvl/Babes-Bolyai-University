#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
	int fd = open("myfifo", O_RDONLY);

	int number = 12;
	int sum = 0;
	
	while(number != 0) {
		read(fd, &number, sizeof(int));
		if (number < 10 || number % 11 == 0 || number % 111 == 0) {
			sum += number;
		}
	}
	printf("Final sum is: %d\n", sum);
	close(fd);
	unlink("myfifo");
	return 0;
}
