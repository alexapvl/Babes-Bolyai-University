#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 1234
#define BUFFER_SIZE 1024
#define SERVER_IP "192.168.1.4" // Replace with the server's IP address

void *receive_messages(void *arg) {
    int sockfd = *((int *)arg);
    char buffer[BUFFER_SIZE];
    
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received > 0) {
            printf("\n[Server]: %s\n", buffer);
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    pthread_t receiver_thread;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid server address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. Type your messages below:\n");

    // Start receiver thread
    if (pthread_create(&receiver_thread, NULL, receive_messages, &sockfd) != 0) {
        perror("Failed to create receiver thread");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Main loop: send messages
    char message[BUFFER_SIZE];
    while (1) {
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0'; // Remove newline character

        if (strlen(message) == 0) continue; // Skip empty messages

        if (send(sockfd, message, strlen(message), 0) < 0) {
            perror("Failed to send message");
        }
    }

    // Clean up
    pthread_cancel(receiver_thread);
    pthread_join(receiver_thread, NULL);
    close(sockfd);
    return 0;
}