// ServerSuma-Iterativ.cpp : Defines the entry point for the console application.

//

 // problema 14.The client sends an array of boolean values. 
 // The server counts how many true values are in the array and returns the count..



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

// on Windows include and link these things

#include<WinSock2.h>

// for uint16_t an so

#include<cstdint>

 

// this is how we can link a library directly from the source code with the VC++ compiler – otherwise got o project settings and link to it explicitly

//#pragma comment(lib,"Ws2_32.lib")

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

              uint16_t suma = 0, n;

              bool arr[1005];

              printf("Listening for incomming connections\n");

              c = accept(s, (struct sockaddr *) &client, &l);

              err = errno;

              if (c < 0) {
                     printf("accept error: %d", err);
                     continue;
              }

              printf("Incomming connected client from: %s:%d", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

              // serving the connected client

                // get n
               int res = recv(c, (char*)&n, sizeof(n), 0);

              //check we got an unsigned short value

                if (res != sizeof(n)) {
                     printf("Error receiving operand\n");
                     closesocket(c);
                     exit(1);
              }
              
              n = ntohs(n);

               // n = 5;

              // get n bool values

              bool r;
              for (int i = 0; i < n; i++) {
                     r = recv(c, (char*)&arr[i], sizeof(arr[i]), 0);
                     if (r != sizeof(arr[i])) {
                            printf("Error receiving operand\n");
                            closesocket(c);
                            exit(2);
                     }
              }

              //decode the value to the local representation
              for (int i = 0; i < n; i++){
                     arr[i] = ntohs(arr[i]);
                     suma += arr[i];
              }


              suma = htons(suma);

              int sum = send(c, (char*)&suma, sizeof(suma), 0);

              if (sum != sizeof(suma)) {
                     printf("Error sending sum\n");
                     closesocket(c);
                     exit(3);
              }

              //on Linux closesocket does not exist but was defined above as a define to close

              closesocket(c);

       }

 

       // never reached

       // Release the Windows Sockets Library

#ifdef _WIN32

       WSACleanup();

#endif

}
