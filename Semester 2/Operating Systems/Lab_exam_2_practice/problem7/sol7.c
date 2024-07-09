#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

pid_t child_pid;

void parent_handler(int sgn) {
	printf("Parent handler\nTransmiting signal to child process\n");
  kill(child_pid, SIGUSR1);
}

void child_handler(int sgn) {
	printf("Child handler");
	exit(0);
}

int main(int argc, char** argv) {
	signal(SIGUSR1, parent_handler);
	
	child_pid = fork();
	if (child_pid == 0) {
		signal(SIGUSR1, child_handler);
		while(1){} // waits for signal
	}

	wait(0); // will wait for child to finish execution
	// i.e. get the signal form another process or the parent process

	return 0;
}
