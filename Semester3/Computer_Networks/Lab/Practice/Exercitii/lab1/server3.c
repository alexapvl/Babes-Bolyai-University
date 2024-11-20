//server3.c


#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

// exists on all platforms

#include <stdio.h>

 

// this section will only be compiled on NON Windows platforms

#ifndef _WIN32

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <netinet/ip.h>

#include <string.h>

#include <arpa/inet.h>

#include <stdbool.h>

#include <stdlib.h>
 

 

#include <unistd.h>

#include <errno.h>

 

#define closesocket close

typedef int SOCKET;

#else




#endif

 

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
       server.sin_port = htons(1234);
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
              long lens;

              //char* s;
              //char* res;


              printf("Listening for incomming connections\n");

              c = accept(s, (struct sockaddr *) &client, &l);

              err = errno;

              if (c < 0) {

                     printf("accept error: %d", err);

                     continue;

              }

              printf("Incomming connected client from: %s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

              // serving the connected client
              // get the length of the string
              int rl = recv(c, (char*)&lens, sizeof(lens), 0);
              
              //check we got an unsigned short value

              if (rl != sizeof(lens)) {
                     printf("Error receiving operand\n");
                     closesocket(c);
                     exit(1);
              }

              lens = ntohl(lens);

              //int rl = recv(c, (char*)&l, sizeof(l), 0);

              // get string
              /*
              char s[lens];

              
              int rs = recv(c, s, lens, 0);  //sizeof(char) * lens

              // check we got a string
              if (rs != lens) {
                     printf("Error receiving operand\n");
                     closesocket(c);
                     exit(2);
              }
              */

              /*
              for (int i = 0; i< lens; i++) {
                     s[i] = ntohs(s[i]);
              }
              

              s[lens] = '\0';
              */

              char* s = (char*)malloc(lens + 1);  // Allocate enough space for the string plus null-terminator
              if (s == NULL) {
                     printf("Memory allocation failed\n");
                     closesocket(c);
                     exit(2);
              }

              // Initialize variables for receiving in chunks
              int received = 0;
              int rs;
              while (received < lens) {
                     // Calculate remaining bytes
                     int bytes_to_receive = lens - received;

                     // Receive data in chunks
                     rs = recv(c, s + received, bytes_to_receive, 0);
                     if (rs < 0) {
                            printf("Error receiving data\n");
                            free(s);
                            closesocket(c);
                            exit(3);
                     }
                     received += rs;
              }

              printf("\nThe length of the string is: %hu\n", strlen(s) - 1);
              printf("lens = %hu\n\n", lens);

              printf("Received string: %s\n\n", s);

              
              //int sum = send(c, (char*)&suma, sizeof(suma), 0);

              for (int i = 0; i < lens/2; i++) {
                     char aux = s[i];
                     s[i] = s[lens-i-1];
                     s[lens-i-1] = aux;
              }

              int rsend = send(c, s, lens, 0);

              if (rsend != lens) {
                     printf("Error sending result\n");
                     free(s);
                     closesocket(c);
                     exit(3);
              }

              //on Linux closesocket does not exist but was defined above as a define to close
              free(s);
              closesocket(c);

       }


}