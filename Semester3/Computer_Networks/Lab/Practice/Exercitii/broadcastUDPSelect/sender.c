#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/select.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define MAX_MESSAGE_LENGTH 1024

// Global socket descriptor
int sockfd;
struct sockaddr_in server_addr;

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in sender_addr;
    socklen_t addr_len = sizeof(sender_addr);

    while (1) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        // Wait for activity on the socket (non-blocking)
        int activity = select(sockfd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select() error");
            continue;
        }

        if (FD_ISSET(sockfd, &readfds)) {
            ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&sender_addr, &addr_len);
            if (bytes_received < 0) {
                perror("Failed to receive message");
                continue;
            }
            buffer[bytes_received] = '\0'; // Null terminate the buffer
            printf("[New message]: %s\n", buffer);
        }
    }
    return NULL;
}

void send_message() {
    char message[MAX_MESSAGE_LENGTH];

    // Send "Client connected" message immediately
    const char *connect_message = "Client connected to the server";
    if (sendto(sockfd, connect_message, strlen(connect_message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to send connection message");
        return;
    }

    while (1) {
        // Using select() to handle input
        fd_set writefds;
        FD_ZERO(&writefds);
        FD_SET(STDIN_FILENO, &writefds);

        // Wait for input (non-blocking)
        int activity = select(STDIN_FILENO + 1, NULL, &writefds, NULL, NULL);
        if (activity < 0) {
            perror("select() error");
            continue;
        }

        if (FD_ISSET(STDIN_FILENO, &writefds)) {
            fgets(message, MAX_MESSAGE_LENGTH, stdin);
            message[strcspn(message, "\n")] = 0;  // Remove newline character

            if (strlen(message) == 0) {
                continue;
            }

            // Send message to server
            if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
                perror("Failed to send message");
                break;
            }
        }
    }
}

int main() {
    int broadcast_enable = 1;

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Allow broadcasting
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("Broadcast not allowed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Setup server address structure for broadcasting
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("255.255.255.255");  // Broadcast address
    server_addr.sin_port = htons(SERVER_PORT);

    printf("Client started. You can send messages.\n");

    // Start the receive thread
    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, receive_messages, NULL) != 0) {
        perror("Failed to create receive thread");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send the "client connected" message to the server
    send_message();

    // Clean up and exit
    pthread_join(receive_thread, NULL);
    close(sockfd);
    return 0;
}
