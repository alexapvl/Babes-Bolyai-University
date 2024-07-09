/*
Write 2 c programs, A and B. A will create 2 prcesses p1 and p2. p1 will
read from the standard input (keyboard) the path to a file f, f can have any number of characters any number of times.
p1 will send to p2, via pipe, the path.
p2 will send to B, via fifo, 5 numbers, each representing the frequency of a large
vowel (A,E,I,O,U) found in the file. B will display the numbers on the
standard output (the console).
The source file must be compiled using gcc with -Wall -g options WITHOUT WARNINGS OR SYNTAX ERRORS!!!
Memory leacks and zombie processes are not allowed.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv) {
	int p1top2[2];
	pipe(p1top2);
	if (fork() == 0) { // p1
		close(p1top2[0]);
		char* path = (char*)malloc(100 * sizeof(char));
		scanf("%s", path);
		int n = strlen(path);
		write(p1top2[1], &n, sizeof(int));
		write(p1top2[1], path, (n * sizeof(char))); 	
		close(p1top2[1]);
		free(path);
		exit(0);
	}

	if (fork() == 0) { // p2
		close(p1top2[1]);
		char* path = (char*)malloc(100 * sizeof(char));
		int n;
		read(p1top2[0], &n, sizeof(int));
		read(p1top2[0], path, (n * sizeof(char))); 

		mkfifo("p2toB", 0600);
		int fd2 = open(path, O_RDONLY);
		free(path);
		int v[5] = {0};
		char c;
		
		while(read(fd2, &c, sizeof(char)) > 0) {
			if (c == 'A') {
				v[0]++;
			} else if (c == 'E') {
				v[1]++;
			} else if (c == 'I') {
				v[2]++;
			} else if (c == 'O') {
				v[3]++;
			} else if (c == 'U') {
				v[4]++;
			}
		}
		close(fd2);

		int fd = open("p2toB", O_WRONLY);
		write(fd, v, (6 * sizeof(int)));
		close(fd);
		exit(0);
	}

	// wait for p1 and p2 to finish
	wait(0);
	wait(0);

	return 0;
}
