#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int client_sockets[MAX_CLIENTS]; // Array to hold client sockets
int num_clients = 0;

void broadcast_message(uint16_t m, uint16_t n, char mat[][1005]) {
  for (int i = 0; i < num_clients; i++) {
    m = htons(m);
    if (send(client_sockets[i], &m, sizeof(m), 0) < 0) {
      perror("Error sending broadcast message = m");
    }
    m = ntohs(m);
    n = htons(n);
    if (send(client_sockets[i], &n, sizeof(n), 0) < 0) {
      perror("Error sending broadcast message = n");
    }
    n = ntohs(n);
    for (int j = 0; j < m; j++)
      for (int k = 0; k < n; k++)
        if (send(client_sockets[i], &mat[j][k], 1, 0) < 0) {
          perror("Error sending broadcast ba ce prost esti");
        }
  }
}

int main(int argc, char* argv[]) {
  uint16_t m, n;
  if (argc != 4) {
    printf("Usage: %s <Port> <m> <n>\n", argv[0]);
    return 1;
  }
  m = atoi(argv[2]);
  n = atoi(argv[3]);
  char mat[1005][1005];
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
      mat[i][j] = '0';
  int server_fd, max_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(client_addr);
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
  server_addr.sin_port = htons(atoi(argv[1]));

  // option to reuse the port
  int opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Bind the socket
  if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
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

  printf("Server is listening on port %d...\n", atoi(argv[1]));

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
    // it returns the nr of ready / available sockets (on how many sockets have been changes)
    if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
      perror("select() failed");
      break;
    }

    // Check if there is a new incoming connection
    if (FD_ISSET(server_fd, &readfds)) {
      int new_client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
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
        uint16_t x, y;
        char c;
        if (recv(client_sockets[i], &x, sizeof(x), 0) <= 0) {
          printf("Client disconnected: %d\n", client_sockets[i]);
          close(client_sockets[i]);
          // Remove the client from the list
          client_sockets[i] = client_sockets[num_clients - 1];
          num_clients--;
          exit(1);
        }
        x = ntohs(x);
        if (recv(client_sockets[i], &y, sizeof(y), 0) <= 0) {
          printf("Client disconnected: %d\n", client_sockets[i]);
          close(client_sockets[i]);
          // Remove the client from the list
          client_sockets[i] = client_sockets[num_clients - 1];
          num_clients--;
          exit(1);
        }
        y = ntohs(y);
        if (recv(client_sockets[i], &c, 1, 0) <= 0) {
          printf("Client disconnected: %d\n", client_sockets[i]);
          close(client_sockets[i]);
          // Remove the client from the list
          client_sockets[i] = client_sockets[num_clients - 1];
          num_clients--;
          exit(1);
        }
        mat[x][y] = c;
        printf("Received from client %d: %hd %hd and character: %c\n", client_sockets[i], x, y, c);
        broadcast_message(m, n, mat); // Broadcast to other clients
      } else {
        continue;
      }
    }
  }

  close(server_fd);
  return 0;
}
