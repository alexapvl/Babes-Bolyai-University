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

              c = accept(s, (struct sockaddr *) &client, (socklen_t *)&l);

              err = errno;

              if (c < 0) {

                     printf("accept error: %d", err);

                     continue;

              }

              printf("Incomming connected client from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

              // serving the connected client

              int nr = 0, sum = 0;

	            int s;

                while (1) {
                    s = recv(c, &nr, sizeof(int), 0);
                    if (s < 0) {
                     printf("Error when recieving number\n");
                     return 1;
                    }
                    nr = ntohl(nr);
                    if (nr == -1) {
                        break;  // Stop on receiving the termination signal
                    }
                    sum += nr;
                    printf("Current sum: %d\n", sum);
                }

                sum = htonl (sum);

                s = send (c, &sum, sizeof (int), 0);

              //on Linux closesocket does not exist but was defined above as a define to close

              closesocket(c);

       }
       return 0;
}