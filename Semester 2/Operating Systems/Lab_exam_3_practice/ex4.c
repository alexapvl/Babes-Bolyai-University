#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int sum = 0;
pthread_barrier_t bar;
pthread_mutex_t m;

void* f(void* a) {
	int* id = (int*)a;
	int v[3];
	for (int i = 0; i < 3; i++) {
		v[i] = rand() % 10 + 1;
	}
	printf("Thread: %d -> %d, %d, %d\n", *id, v[0], v[1], v[2]);
	pthread_barrier_wait(&bar);
	printf("AAAAAAAAAAA");
	return NULL;
}

int main(int argc, char** argv) {
		int n = atoi(argv[1]);
		pthread_t t[n];

		pthread_mutex_init(&m, NULL);
		pthread_barrier_init(&bar, NULL, n);

		for (int i = 0; i < n; i++) {
			int* id = (int*)malloc(sizeof(int));
			*id = i;
			pthread_create(&t[i], NULL, f, (void*)id); 
		}

		for (int i = 0; i < n; i++) {
			pthread_join(t[i], NULL);
		}

		pthread_barrier_destroy(&bar);
		pthread_mutex_destroy(&m);

	return 0;
}
