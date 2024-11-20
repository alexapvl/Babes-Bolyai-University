/*
client problem 1 lab 3
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For sleep()
#include <time.h>
#include <bits/time.h>

void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    int sock, length, n;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[256];
    socklen_t fromlen;
    struct timespec start, end;

    if (argc != 3) {
        printf("Usage: %s <server_name> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");

    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp == 0) error("Unknown host");

    bcopy((char *)hp->h_addr_list[0], (char *)&server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));

    length = sizeof(struct sockaddr_in);
    fromlen = sizeof(struct sockaddr_in);

    srand(time(NULL));

    while (1) {
        // Generate random content for PING
        snprintf(buffer, sizeof(buffer), "PING %d", rand());

        // Record the start time
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Send the PING message
        n = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server, length);
        if (n < 0) error("sendto");

        // Receive the echoed message from the server
        n = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&from, &fromlen);
        if (n < 0) error("recvfrom");

        // Record the end time and compute RTT
        clock_gettime(CLOCK_MONOTONIC, &end);
        double rtt = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;

        // Display the echoed message and RTT
        buffer[n] = '\0';
        printf("Received echo: %s\n", buffer);
        printf("Round-Trip Time: %.3f ms\n", rtt);

        // Wait for a second before sending the next PING
        sleep(1);
    }

    close(sock);
    return 0;
}