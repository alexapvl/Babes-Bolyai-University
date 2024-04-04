#include <stdio.h>

int main(int argc, char** argv){
	int i;
	FILE* f;
	
	f = popen("less", "w");
	for(i = 99 ;i >= 0; i--) {
		fprintf(f, "%d bottles of beer on the wall\n", i);
	}

	pclose(f);
	return 0;
}
