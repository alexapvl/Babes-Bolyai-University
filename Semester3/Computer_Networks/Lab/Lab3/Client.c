#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {

       int c;
       struct sockaddr_in server;
       uint16_t a, b, suma;
       c = socket(AF_INET, SOCK_STREAM, 0);
       if (c < 0) {
          printf("Eroare la crearea socketului client\n");
          return 1;
       }
       memset(&server, 0, sizeof(server));
       server.sin_port = htons(3002);
       server.sin_family = AF_INET;
       server.sin_addr.s_addr = inet_addr("172.30.254.79");
       if (connect(c, (struct sockaddr *) &server, sizeof(server)) < 0) {
          printf("Eroare la conectarea la server\n");
          return 1;
       }

    int n, i = 0, nr, s;
    int arr[100];

    while (1) {
        printf("Give a number: ");
        scanf("%d", &nr);
        arr[i] = nr;
        i++;
        if (nr == -1) {
            break;
        }
    }

    n = i;

    for (i = 0 ; i<n ; i++) {
        nr = htonl(arr[i]);
        s = send (c, &nr, sizeof(nr), 0);
        printf ("Sent %d bytes to the server\n", s);

    }

    int sum;

    //Recieving data from server

    s = recv (c, &sum, sizeof (int), 0);

    printf ("Recived %d bytes from the server\n", s);
    
    //Converting data from network arhitecture to local arhitecture.

    sum = ntohl(sum);

    printf ("Recived from the server: %d\n", sum);

    close(c);
}