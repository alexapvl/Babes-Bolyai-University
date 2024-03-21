#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
	int i;
	for(i = 0; i < 3; i++){
		printf("before %d %d\n" , getpid(), getppid());
		fork(); // creates a copy of the process in memory which picks up the execution of the program
		// from the same point at which the fork was made
		printf("after %d %d\n", getpid(), getppid());
	}
	return 0;
	// getpid() -> get the current process ID
	// getppid() -> get parent process ID
}


// fork bomb
//	while(1) {
//		fork();	
//	}

