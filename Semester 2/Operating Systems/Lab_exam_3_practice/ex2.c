#include <stdio.h>
#include <pthread.h>

int n = 1;

void* f(void* a) {
	for (int i = 0; i < n; i++) {
		printf("%s\n", (char*)a); // we need to cast a as a char* to not get errors
	}

	return NULL;
}

int main(int argc, char** argv) {
	pthread_t ta, tb;
	
	if (argc > 1) {
		sscanf(argv[1], "%d", &n);
	}

	pthread_create(&ta, NULL, f, "fa");
	pthread_create(&tb, NULL, f, "fb"); // notice that the parameters passed here are character strings, aka char*

	for(int i = 0; i < n; i++) {
		printf("main\n");
	}

	pthread_join(ta, NULL);
	pthread_join(tb, NULL);

	return 0;
}
