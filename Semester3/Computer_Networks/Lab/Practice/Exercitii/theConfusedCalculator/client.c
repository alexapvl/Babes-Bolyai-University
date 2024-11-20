#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define max 100
 
int main() {
  int c, cod;
  char sol[max];
  // Observatie: Deoarece dimensiunea tipului int difera de la platforma la platforma,
  // (spre exemplu, in Borland C in DOS e reprezentat pe 2 octeti, iar in C sub Linux pe
  // 4 octeti) este necesara utilizarea unor tipuri intregi standard. A se vedea
  // stdint.h.
  struct sockaddr_in server;
  char s[max];
  
  c = socket(PF_INET, SOCK_STREAM, 0);
  if (c < 0) {
    fprintf(stderr, "Eroare la creare socket client.\n");
    return 1;
  }
  
  memset(&server, 0, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = htons(4321);
  server.sin_addr.s_addr = inet_addr("192.168.1.4");
  
  cod = connect(c, (struct sockaddr *) &server, sizeof(struct sockaddr_in));
  if (cod < 0) {
    fprintf(stderr, "Eroare la conectarea la server.\n");
    return 1;
  }
  
  printf("Write the exercise: ");
  fgets(s, max, stdin);
  
  // !!! important - trimit lungimea sirului + 1 pentru a trimite pe socket si caracterul NULL (0) care marcheaza sfarsitului sirului.
  // paragraful 1 din protocol
  cod = send(c, s, strlen(s) + 1, 0);
  if (cod != strlen(s) + 1) {
    fprintf(stderr, "Eroare la trimiterea datelor la server.\n");
    return 1;
  }
  
  // paragraful 5 din protocol
  int i = 0;
  do {
    cod = recv(c, &sol[i], 1, 0);
    printf("Am primit %d caractere.\n", cod);
    i++;
 
    if (cod == 1) // citire cu succes a unui caracter
      alarm(10);  // resetam timerul si asteptam din nou 10 secunde urmatorul caracter
      
    if (cod < 1) {
      fprintf(stderr, "Eroare la primirea datelor de la client.\n");
      return 1;
    }
  }
  while (sol[i - 1] != 0);
  
  
  printf("%s\n", sol);
  
  close(c);
}
 
 