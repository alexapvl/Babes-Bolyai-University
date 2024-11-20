#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MYPORT 9009    // The port users will be connecting to
#define BUFSIZE 256    // Buffer size for messages

int main() {
    int sock;
    struct sockaddr_in recv_addr;
    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char recvbuff[BUFSIZE];
    int broadcast = 1;  // Enable broadcasting on the socket

    // Create socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Error in socket creation");
        return 1;
    }

    // Set socket options for broadcasting
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        perror("Error in setting broadcast option");
        close(sock);
        return 1;
    }

    // Prepare the receiving address (listen for messages)
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(MYPORT);
    recv_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all available interfaces

    // Bind the socket to the address and port
    if (bind(sock, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) < 0) {
        perror("Error in binding");
        close(sock);
        return 1;
    }

    // Continuously listen for incoming messages
    while (1) {
        // Receive a broadcast message
        if (recvfrom(sock, recvbuff, BUFSIZE, 0, (struct sockaddr*)&sender_addr, &addr_len) < 0) {
            perror("Error in receiving message");
            close(sock);
            exit(1);
        }

        recvbuff[strlen(recvbuff)] = '\0'; // Null-terminate the received message
        printf("Received message: %s\n", recvbuff);
    }

    // Clean up and close the socket
    close(sock);
    return 0;
}
