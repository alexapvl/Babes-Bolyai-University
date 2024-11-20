#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

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
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&sender_addr, &addr_len);
        if (bytes_received < 0) {
            perror("Failed to receive message");
            continue;
        }
        printf("[New message]: %s\n", buffer);
    }
    return NULL;
}

void send_message(const char *message) {
    // Send message to server (broadcast to all clients)
    if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Failed to send message");
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

    // Send a "client connected" message to the server to register the client
    send_message("Client connected to the server");

    // Send messages from user input
    char message[MAX_MESSAGE_LENGTH];
    while (1) {
        fgets(message, MAX_MESSAGE_LENGTH, stdin);
        message[strcspn(message, "\n")] = 0;  // Remove newline character

        if (strlen(message) == 0) {
            continue;
        }

        // Send message to server
        send_message(message);
    }

    // Clean up and exit
    pthread_join(receive_thread, NULL);
    close(sockfd);
    return 0;
}
