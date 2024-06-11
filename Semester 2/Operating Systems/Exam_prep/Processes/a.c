#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

int main(int argc, char** argv) {
	if(fork() != fork())
	{
		printf("We are in the process with PID: %d\n", getpid());
		fork();
	}
	
	printf("Pid: %d -- PPid: %d\n", getpid(), getppid());
	
	return 0;
}
