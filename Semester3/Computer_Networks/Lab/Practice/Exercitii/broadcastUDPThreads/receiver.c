#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

struct sockaddr_in clients[MAX_CLIENTS];
int num_clients = 0;

// Function to broadcast message to all clients except the sender
void broadcast_message(int sockfd, const char *message, struct sockaddr_in *sender) {
    for (int i = 0; i < num_clients; i++) {
        // Skip sending the message to the sender
        if (clients[i].sin_addr.s_addr == sender->sin_addr.s_addr &&
            clients[i].sin_port == sender->sin_port) {
            continue;
        }

        if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&clients[i], sizeof(clients[i])) < 0) {
            perror("Broadcast message failed");
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running and waiting for clients...\n");

    // Main server loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received < 0) {
            perror("Receive failed");
            continue;
        }

        // Check if the client is already registered
        int is_new_client = 1;
        for (int i = 0; i < num_clients; i++) {
            if (clients[i].sin_addr.s_addr == client_addr.sin_addr.s_addr &&
                clients[i].sin_port == client_addr.sin_port) {
                is_new_client = 0;
                break;
            }
        }

        // If it's a new client, add to client list
        if (is_new_client && num_clients < MAX_CLIENTS) {
            clients[num_clients++] = client_addr;
            printf("New client joined: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        }

        // Do not broadcast the "connection" message back to the client
        if (strcmp(buffer, "Client connected to the server") != 0) {
            // Broadcast the received message to all clients except the sender
            printf("Received message from %s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);
            broadcast_message(sockfd, buffer, &client_addr);
        }
    }

    close(sockfd);
    return 0;
}
