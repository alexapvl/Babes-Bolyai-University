#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Usage: ./%s name_of_fifo\n", argv[1]);
		exit(1);
	}
	char* file_name = NULL;
	size_t len = 0;
	int no_char_read; // will store the number of characters read in getline
	printf("Enter file name from which to read: ");

	no_char_read = getline(&file_name, &len, stdin);
	if (no_char_read == -1) {
		perror("getline");
		free(file_name); // we need to free it because malloc is used internally in getline!
		exit(2);
	}
	if (file_name[no_char_read - 1] == '\n')
		file_name[no_char_read - 1] = '\0';
	if (mkfifo(argv[1], 0600) == -1) { // make the fifo to write to it
		perror("error on making fifo");
		free(file_name);
		exit(3);
	}
	int fifofd = open(argv[1], O_WRONLY);
	if (fifofd == -1) {
		perror("error on opening fifo");
		free(file_name);
		exit(4);
	}
	FILE* fd = fopen(file_name, "r");
	char c;
	while (fscanf(fd, "%c", &c) > 0) {
		if (c >= '0' && c <= '9')
			if (write(fifofd, &c, sizeof(char)) == -1) {
				perror("error on writing to fifo");
				free(file_name);
				exit(5);
			}
	}
	free(file_name);
	fclose(fd);
	close(fifofd);
	return 0;
}
