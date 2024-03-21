#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
	int pid;
	
	printf("before\n");
	pid = fork();
	if(pid == 0) {
		printf("Child specific code pid = %d fork return = %d\n", getpid(), pid);
		exit(0);
	}
	printf("Parent specific code PID = %d\n", pid);
	wait(0); // waits for the first child to finish
	return 0;
}

