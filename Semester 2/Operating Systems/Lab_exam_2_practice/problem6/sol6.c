#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Usage: ./%s number_of_child_processes\n", argv[0]);
		return 1;
	}

	printf("Parent PID: %d\n", getpid());

	for(int i = 0; i < argc; i++) {
		pid_t childPid = fork();
		if (childPid == 0) { // child
			printf("PID: %d -- PPID: %d\n", getpid(), getppid());
			exit(0);
		}
		printf("Child PID: %d\n", childPid);

	}

	for(int i = 0; i < argc; i++) {
		wait(0);
	}

	return 0;
}
