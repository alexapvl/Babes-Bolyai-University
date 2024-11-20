#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MYPORT 1234    // The port users will be connecting to
#define BROADCAST_ADDR "255.255.255.255"  // Broadcast address
#define BUFSIZE 256    // Buffer size for messages

int main() {
    int sock;
    struct sockaddr_in send_addr;
    struct sockaddr_in recv_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    char sendMSG[BUFSIZE];
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

    // Prepare the sending address (broadcast address)
    memset(&send_addr, 0, sizeof(send_addr));
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(MYPORT);
    send_addr.sin_addr.s_addr = inet_addr(BROADCAST_ADDR);  // Use broadcast address

    // Prepare the receiving address
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

    // Continuously send and receive messages
    while (1) {
        // Get user input message to broadcast
        printf("Enter message to broadcast (or type 'exit' to quit): ");
        fgets(sendMSG, BUFSIZE, stdin);

        // If the user types "exit", break the loop and exit
        if (strncmp(sendMSG, "exit", 4) == 0) {
            break;
        }

        // Send the message to the broadcast address
        if (sendto(sock, sendMSG, strlen(sendMSG), 0, (struct sockaddr*)&send_addr, sizeof(send_addr)) < 0) {
            perror("Error in sending message");
            close(sock);
            exit(1);
        }
        printf("Message broadcasted: %s\n", sendMSG);

        // Receive a message from any sender
        char recvbuff[BUFSIZE];
        if (recvfrom(sock, recvbuff, BUFSIZE, 0, (struct sockaddr*)&recv_addr, &addr_len) < 0) {
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
