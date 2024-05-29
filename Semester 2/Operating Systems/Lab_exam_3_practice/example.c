#include <stdio.h>
#include <pthread.h>

void* f(void* a) {
	printf("f\n");
	return NULL;
}

int main(int argc, char** argv) {
	pthread_t t; // thread type object
	pthread_create(&t, NULL, f, NULL); // second NULL is for the arguments of f
	printf("main\n");
	pthread_join(t, NULL); // the NULL is for where to return the value returned by f
	return 0;
}
