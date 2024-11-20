#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define MAX_MESSAGE_LENGTH 1024

int sockfd;
struct sockaddr_in server_addr;

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    while (1) {
        ssize_t bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received < 0) {
            perror("recv() failed");
            continue;
        }
        buffer[bytes_received] = '\0';
        printf("[New message]: %s\n", buffer);
    }
    return NULL;
}

void send_message() {
    char message[MAX_MESSAGE_LENGTH];

    while (1) {
        fgets(message, MAX_MESSAGE_LENGTH, stdin);
        message[strcspn(message, "\n")] = 0;  // Remove newline character

        if (strlen(message) == 0) {
            continue;
        }

        if (send(sockfd, message, strlen(message), 0) < 0) {
            perror("Failed to send message");
            break;
        }
    }
}

int main() {
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server address
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. You can start sending messages.\n");

    // Start the receive thread
    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, receive_messages, NULL) != 0) {
        perror("Failed to create receive thread");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send messages in the main thread
    send_message();

    // Wait for the receive thread to finish
    pthread_join(receive_thread, NULL);

    close(sockfd);
    return 0;
}
