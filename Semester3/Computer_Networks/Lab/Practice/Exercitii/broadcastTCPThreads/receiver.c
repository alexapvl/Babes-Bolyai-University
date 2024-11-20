#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 1234
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

typedef struct {
    int sockfd;
    struct sockaddr_in client_addr;
} ClientInfo;

// Global variables
ClientInfo clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t client_mutex = PTHREAD_MUTEX_INITIALIZER;

// Broadcast a message to all connected clients
void broadcast_message(const char *message, int sender_sockfd) {
    pthread_mutex_lock(&client_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].sockfd != sender_sockfd) {
            send(clients[i].sockfd, message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&client_mutex);
}

// Handle communication with a single client
void *handle_client(void *arg) {
    ClientInfo client = *((ClientInfo *)arg);
    char buffer[BUFFER_SIZE];

    printf("Client connected: %s:%d\n",
           inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(client.sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            printf("Client disconnected: %s:%d\n",
                   inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port));
            close(client.sockfd);

            // Remove the client from the list
            pthread_mutex_lock(&client_mutex);
            for (int i = 0; i < client_count; i++) {
                if (clients[i].sockfd == client.sockfd) {
                    clients[i] = clients[client_count - 1];
                    client_count--;
                    break;
                }
            }
            pthread_mutex_unlock(&client_mutex);
            break;
        }

        printf("[Message from %s:%d]: %s\n",
               inet_ntoa(client.client_addr.sin_addr), ntohs(client.client_addr.sin_port), buffer);

        // Broadcast the message to all clients
        broadcast_message(buffer, client.sockfd);
    }

    return NULL;
}

int main() {
    int server_sockfd;
    struct sockaddr_in server_addr;

    // Create socket
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket to the address and port
    if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sockfd);
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_sockfd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        close(server_sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    // Main loop: accept connections and create threads for clients
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (client_sockfd < 0) {
            perror("Failed to accept connection");
            continue;
        }

        // Add client to the list
        pthread_mutex_lock(&client_mutex);
        if (client_count >= MAX_CLIENTS) {
            printf("Max clients reached. Connection rejected: %s:%d\n",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            close(client_sockfd);
            pthread_mutex_unlock(&client_mutex);
            continue;
        }

        clients[client_count].sockfd = client_sockfd;
        clients[client_count].client_addr = client_addr;
        client_count++;
        pthread_mutex_unlock(&client_mutex);

        // Create a thread to handle the client
        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, &clients[client_count - 1]) != 0) {
            perror("Failed to create client thread");
            close(client_sockfd);
        }
        pthread_detach(client_thread);
    }

    // Clean up
    close(server_sockfd);
    return 0;
}