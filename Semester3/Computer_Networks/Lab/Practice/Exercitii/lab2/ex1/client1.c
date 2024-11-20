#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#define max 100
 
int main() {
  int c, cod;
  int32_t r;
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
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  cod = connect(c, (struct sockaddr *) &server, sizeof(struct sockaddr_in));
  if (cod < 0) {
    fprintf(stderr, "Eroare la conectarea la server.\n");
    return 1;
  }
  
  printf("Dati o comanda pentru trimis la server: ");
  fgets(s, max, stdin);
  
  // !!! important - trimit lungimea sirului + 1 pentru a trimite pe socket si caracterul NULL (0) care marcheaza sfarsitului sirului.
  // paragraful 1 din protocol
  int32_t slen = strlen(s);
  int32_t cslen=htonl(slen);
  
  send(c,&cslen,sizeof(int32_t),0);

  cod = send(c, s, slen + 1, 0);


  if (cod != slen + 1) {
    fprintf(stderr, "Eroare la trimiterea datelor la server.\n");
    return 1;
  }
  
  char *output = NULL;
    int output_len = 0;
    int received_bytes = 0;
    char buffer[1024];
    int32_t exit_code;


    if(recv(c, &output_len, sizeof(int32_t), 0) < sizeof(int32_t)) {
        printf("Error receiving the command length");
    }
    output_len = ntohl(output_len);

    if(output_len > 1) {
        // Allocate memory for command string
        output= (char *)malloc(output_len + 1); // +1 for null terminator
        if (output == NULL) {
            perror("Eroare la alocarea memoriei pentru comanda.");
            close(c);
            exit(1);
        }

        memset(output, 0, output_len + 1);  // Initialize to zero


        while (received_bytes < output_len) {
            int chunk = recv(c, buffer, sizeof(buffer), 0);
            if (chunk <= 0) {
                printf("Eroare la citirea comenzii de la client.\n");
                free(output);
                close(c);
                exit(1);
            }
            strncat(output, buffer, chunk);
            received_bytes += chunk;
        }
        printf("The output received is: %s",output);

        free(output);
    }
    else printf("The command is not valid!\n");
    
  
    if(recv(c, &exit_code, sizeof(int32_t), 0) != sizeof(int32_t)) {
        printf("Error receiving the exit code");
        close(c);
        exit(2);
    }
    exit_code = ntohl(exit_code);
    printf("\n\nThe exit code is %hu\n", exit_code);
  
  close(c);
}