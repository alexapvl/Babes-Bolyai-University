#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS];  // Array to hold client sockets
int num_clients = 0;

void broadcast_message(int sender_fd, const char *message) {
    for (int i = 0; i < num_clients; i++) {
        // Don't send the message to the sender
        if (client_sockets[i] == sender_fd) {
            continue;
        }
        if (send(client_sockets[i], message, strlen(message), 0) < 0) {
            perror("Error sending broadcast message");
        }
    }
}

int main() {
    int server_fd, max_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    fd_set readfds;

    // Create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind the socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", SERVER_PORT);

    // Set the initial max file descriptor
    max_fd = server_fd;

    while (1) {
        // Clear the readfds set and add the server socket
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        // Add client sockets to the set
        for (int i = 0; i < num_clients; i++) {
            FD_SET(client_sockets[i], &readfds);
            if (client_sockets[i] > max_fd) {
                max_fd = client_sockets[i];
            }
        }

        // Wait for activity on one of the sockets
        if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
            perror("select() failed");
            break;
        }

        // Check if there is a new incoming connection
        if (FD_ISSET(server_fd, &readfds)) {
            int new_client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
            if (new_client_fd < 0) {
                perror("Accept failed");
                continue;
            }

            // Add the new client to the client socket list
            if (num_clients < MAX_CLIENTS) {
                client_sockets[num_clients++] = new_client_fd;
                printf("New client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
            } else {
                printf("Max clients reached, rejecting connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                close(new_client_fd);
            }
        }

        // Check for activity on each client socket
        for (int i = 0; i < num_clients; i++) {
            if (FD_ISSET(client_sockets[i], &readfds)) {
                ssize_t bytes_received = recv(client_sockets[i], buffer, BUFFER_SIZE, 0);
                if (bytes_received <= 0) {
                    // Client disconnected or error
                    if (bytes_received == 0) {
                        printf("Client disconnected: %d\n", client_sockets[i]);
                    } else {
                        perror("recv() failed");
                    }
                    close(client_sockets[i]);
                    // Remove the client from the list
                    client_sockets[i] = client_sockets[num_clients - 1];
                    num_clients--;
                } else {
                    buffer[bytes_received] = '\0';
                    printf("Received from client %d: %s\n", client_sockets[i], buffer);
                    broadcast_message(client_sockets[i], buffer);  // Broadcast to other clients
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
