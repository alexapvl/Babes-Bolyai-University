#include <sys/types.h>

#include <sys/socket.h>

#include <stdio.h>

#include <netinet/in.h>

#include <netinet/ip.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>

#include <stdlib.h>

 

int main() {

       int c;
       struct sockaddr_in server;
       uint16_t arr[1005], suma, n;

 

       c = socket(AF_INET, SOCK_STREAM, 0);
       if (c < 0) {
              printf("Eroare la crearea socketului client\n");
              return 1;
       }

       memset(&server, 0, sizeof(server));
       server.sin_port = htons(1234);
       server.sin_family = AF_INET;
       server.sin_addr.s_addr = inet_addr("192.168.1.4");

       if (connect(c, (struct sockaddr *) &server, sizeof(server)) < 0) {
              printf("Eroare la conectarea la server\n");
              return 1;
       }

       printf("n = ");
       scanf("%hu", &n);
       uint16_t m = n;
       m = htons(m);

 
       for (int i = 0; i < n; i++) {
              printf("i = ");
              scanf("%hu", &arr[i]);
       }

       for (int i = 0; i < n;i++) {
              arr[i] = htons(arr[i]);
       }

       if(send(c, &m, sizeof(m), 0) < sizeof(m)) {
              printf("Error sending the length of the array");
              close(c);
              return 1;
       }

       for (int i = 0; i < n;i++) {
              if(send(c, &arr[i], sizeof(arr[i]), 0) < sizeof(arr[i])) {
                     printf("Error sending an element of the array");
                     close(c);
                     return 1;
              }  
       }

       // recv(c, &suma, sizeof(suma), 0);

       ssize_t total_bytes_received = 0;
       size_t expected_size = sizeof(suma);

       while (total_bytes_received < expected_size) {
              ssize_t bytes_received = recv(c, (char*)&suma + total_bytes_received, expected_size - total_bytes_received, 0);
    
              if (bytes_received < 0) {
                     printf("Error receiving data\n");
                     close(c);
                     exit(3);
              } 
              total_bytes_received += bytes_received;
       }

       suma = ntohs(suma);

       printf("Suma este %hu\n", suma);

       close(c);

}

 

 
