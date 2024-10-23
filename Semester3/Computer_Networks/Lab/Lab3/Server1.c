#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

 

#define closesocket close

typedef int SOCKET;
 

int main() {

       SOCKET s;

       struct sockaddr_in server, client;

       int c, l, err;

 

       s = socket(AF_INET, SOCK_STREAM, 0);

       if (s < 0) {

              printf("Eroare la crearea socketului server\n");

              return 1;

       }

 

       memset(&server, 0, sizeof(server));

       server.sin_port = htons(3002);

       server.sin_family = AF_INET;

       server.sin_addr.s_addr = INADDR_ANY;

 

       if (bind(s, (struct sockaddr *) &server, sizeof(server)) < 0) {

              perror("Bind error:");

              return 1;

       }

 

       listen(s, 5);

 

       l = sizeof(client);

       memset(&client, 0, sizeof(client));

 

       while (1) {

              uint16_t a, b, suma;

              printf("Listening for incomming connections\n");

              c = accept(s, (struct sockaddr *) &client, &l);

              err = errno;

              if (c < 0) {

                     printf("accept error: %d", err);

                     continue;

              }

              printf("Incomming connected client from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

              // serving the connected client
              size_t length;

              int cod = recv(c, &length, sizeof(size_t), 0);

              if (cod < -1) {
                     fprintf(stderr, "Eroare la primirea datelor la server. (citire lungime)\n");
                     return 1;
              }

              length = ntohl(length);

              char s[length];
              printf("Am primit lungimea %d\n", length);
              

              char letter;  
              int i = 0;

              while (i < length) {
                     //printf("i = %d\n", i);
                     int cod = recv(c, &letter, 1, 0);

                     //printf("am primit %c\n", letter);

                     if (cod < -1) {
                            fprintf(stderr, "Eroare la primirea datelor la server. (citire lungime)\n");
                            break;
                     }

                     s[i] = letter;
                     i++;

              }

              printf("Okay we got the word %s\n", s);

              for (i = 0; i < length/2 ; i++) {
                     char aux;
                     aux = s[i];
                     s[i] = s[length - i - 2];
                     s[length - i - 2] = aux;
              }

              printf("Okay we made the word %s, planning to send.\n", s);

              cod = send(c, s, strlen(s), 0);

              if (cod < -1) {
                     fprintf(stderr, "Eroare la trimiterea datelor la server.\n");
                     break;
              }

              //on Linux closesocket does not exist but was defined above as a define to close

              closesocket(c);

       }

 

       // never reached

       // Release the Windows Sockets Library
}