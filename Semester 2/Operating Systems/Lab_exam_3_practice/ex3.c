#include <stdio.h>
#include <pthread.h>

struct arg_t {
	char* name;
	int count;
};

void* f(void* a) {
	struct arg_t* x = (struct arg_t*)a;
	for( int i = 0; i < x->count; i++) {
		printf("%s\n", x->name);
	}
	return NULL;
}

int main(int argc, char** argv) {
	int n = 1;
	pthread_t ta, tb;
	struct arg_t aa, ab;

	if (argc > 1) {
		sscanf(argv[1], "%d", &n);
	}

	aa.name = "fa"; ab.name = "fb";
	aa.count = n; ab.name = n;

	pthread_create(&ta, NULL, f, &aa);
	pthread_create(&tb, NULL, f, &ab);

	for (int i = 0; i < n; i++) {
		printf("main\n");
	}
	
	pthread_join(ta, NULL);
	pthread_join(tb, NULL);
	

	return 0;
}

