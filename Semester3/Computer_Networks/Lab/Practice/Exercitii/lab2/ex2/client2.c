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
  
  fprintf(stderr,"Input a file path: ");
  fgets(s, max, stdin);
  
  // !!! important - trimit lungimea sirului + 1 pentru a trimite pe socket si caracterul NULL (0) care marcheaza sfarsitului sirului.
  // paragraful 1 din protocol
  int32_t slen = strlen(s);
  int32_t cslen=htonl(slen);
  
  if (send(c, &cslen, sizeof(int32_t), 0) != sizeof(int32_t)) {
      fprintf(stderr, "Error sending string length to server.\n");
      close(c);
      return 1;
  }

  cod = send(c, s, slen + 1, 0);


  if (cod != slen + 1) {
    fprintf(stderr, "Eroare la trimiterea datelor la server.\n");
    close(c);
    return 1;
  }
  
    char *file_content = NULL;
    int file_len = 0;
    int received_bytes = 0;
    char buffer[1024];


    if(recv(c, &file_len, sizeof(int32_t), 0) < sizeof(int32_t)) {
        fprintf(stderr, "Error receiving the command length");
        close(c);
        exit(1);
    }
    file_len = ntohl(file_len);

    printf("The file length is %d\n", file_len);

    if (file_len == -1) {
        fprintf(stderr, "The file does not exist on the server.\n");
        close(c);
        return 1;
    }


    // Allocate memory for command string
    file_content = (char *)malloc(file_len + 1); // +1 for null terminator
    if (file_content == NULL) {
        perror("Eroare la alocarea memoriei.");
        close(c);
        exit(1);
    }

    memset(file_content, 0, file_len + 1);  // Initialize to zero


    while (received_bytes < file_len) {
        int chunk = recv(c, buffer, sizeof(buffer), 0);
        if (chunk <= 0) {
            fprintf(stderr,"Eroare la citirea fisierului.\n");
            free(file_content);
            close(c);
            exit(1);
        }
        memcpy(file_content + received_bytes, buffer, chunk);
        //strncat(file_content, buffer, chunk);
        received_bytes += chunk;
    }
    

    /// store the content in a file with the same name as the input file with the suffix –copy appended (ex: for f.txt => f.txt-copy)
    char filename[max];
    strcpy(filename, s);
    char* dot_pos = strchr(filename, '.');
    if(dot_pos != NULL) {
        char aux[max] = "";
        strncpy(aux, filename, (int)(dot_pos - filename));
        strcat(aux, "-copy");
        strcat(aux, dot_pos);
        strcpy(filename, aux);
    } else strcat(filename, "-copy");

    FILE* new_file = fopen(filename, "w");
    if(new_file == NULL) {
      perror("Error opening the file");
      free(file_content);
      close(c);
      exit(1);
    }

    fwrite(file_content, sizeof(char), file_len, new_file);
    fclose(new_file);

    free(file_content);
    
    
  
  close(c);
}