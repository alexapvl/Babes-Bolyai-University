/*
Se se implementeze doua procese A si B. Procesul A citeste de la tastatura numere < 1000,
 pana la intalnirea valorii 0 si i le transmite procesului B. B verifica care dintre 
acele numere este scris cu o singura cifra (ex: 7, 77, 777), calculeaza suma acelor 
numere si o transmite procesului A. A afiseaza suma calculata de B. Codul C trebuie 
sa compileze fara erori sau warning-uri si sa fie indentat.
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
	mkfifo("myfifo", 0600);
	
	int fd = open("myfifo", O_WRONLY);
	
	int number = -1;

	while(number != 0) {
		scanf("%d", &number);
		if(number < 1000) {
			write(fd, &number, sizeof(int));
		} else {
			printf("Number must be < 1000\n");
		}
	}

	close(fd);
	return 0;
}
