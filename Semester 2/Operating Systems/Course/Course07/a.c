#include "h.h"

int main(int argc, char** argv) {
	int shmid;
	struct absp* x;
	
	shmid = shmget(134332, sizeof(struct absp), IPC_CREAT|0600); 
	// bitwise OR with the flag and the permisions
	x = shmat(shmid, 0, 0); // pointer to the struct in memory

	srand(time(NULL));
	// time(NULL) -> current time in seconds since ~1970
	while(1) {
		x->a = rand() % 100;
		x->b = rand() % 100;
		if(x->p == x->s) {
				break;
		}
	}

	shmdt(x);
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}
