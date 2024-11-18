/* The server chooses a random float number <SRF>. Run multiple clients.
Each client chooses a random float number <CRF> and send it to the server.
When the server does not receive any incoming connection for at least 10
seconds it chooses the client that has guessed the best approximation
(is closest) for its own number and sends it back the message “You have
the best guess with an error of <SRV>-<CRF>”. It also sends to each other
client the string “You lost !”. The server closes all connections after this. */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define closesocket close
typedef int SOCKET;

float ntohf(uint32_t p) {
  float f;
  // convert from network byte order
  p = ntohl(p);
  // reinterpret int bits as float bits
  memcpy(&f, &p, sizeof(f));
  return f;
}

float generateRandomFloat(float a) {
  srand((unsigned int)clock());
  return ((float)rand() / (float)RAND_MAX) * a;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <Port>", argv[0]);
    return 1;
  }

  SOCKET server_socket;
  struct sockaddr_in server_address, client_address;
  socklen_t client_address_length;

  // Create a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("Socket creation error:");
    return 1;
  }

  // Setup server address struct
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_port = htons(atoi(argv[1])); // Set port
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY; // Accept any incoming

  // Bind the socket
  if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    perror("Bind error:");
    return 1;
  }

  // Listen for incoming connections
  listen(server_socket, 5);

  // Initialize the client address struct length
  client_address_length = sizeof(struct sockaddr_in);
  memset(&client_address, 0, sizeof(struct sockaddr_in));

  // Generate the random float number
  float SRF = generateRandomFloat(10.0);
  printf("This is the random number generated: %f\n", SRF);

  int fdmax = server_socket;
  fd_set master, read_fds;
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  FD_SET(server_socket, &master);
  struct timeval timeout;
  timeout.tv_sec = 10; // 10 seconds until timeout if no descriptors are ready
  timeout.tv_usec = 0;
  int numberOfReadySockets;
  int new_client_socket, winner_client;
  float min_dif = 1000.00;
  // Main loop to accept and handle clients
  while (1) {
    read_fds = master; // copy because the read_fds will be changed by the select call
    numberOfReadySockets = select(fdmax + 1, &read_fds, NULL, NULL, &timeout);
    if (numberOfReadySockets < 0) {
      perror("Error on select()");
      // close all sockets
      for (int socket = 0; socket <= fdmax; ++socket) {
        if (FD_ISSET(socket, &master))
          close(socket);
      }
      return 1;
    } else if (numberOfReadySockets == 0) {
      printf("No sockets have any data pending, so we close the application and display the winner\n");
      for (int socket = 0; socket <= fdmax; ++socket) {
        if (socket == server_socket)
          continue;
        else if (socket == winner_client) {
          // sent message to winner client
          char message[100];
          sprintf(message, "You have the best guess with an error of %f", min_dif);
          message[strlen(message)] = '\0';
          size_t messageLen = strlen(message);
          messageLen = htons(messageLen);
          send(socket, &messageLen, sizeof(size_t), 0);
          send(socket, message, strlen(message), 0);
        } else if (FD_ISSET(socket, &master)) {
          // send message to loser client
          char message[100];
          sprintf(message, "You lost!");
          message[strlen(message)] = '\0';
          size_t messageLen = strlen(message);
          send(socket, &messageLen, sizeof(size_t), 0);
          send(socket, message, strlen(message), 0);
        }
        close(socket); // close all connections with all clients, including the server socket
      }
      return 0;
    } else { // we have sockets ready
      for (int socket = 0; socket <= fdmax; ++socket) {
        if (FD_ISSET(socket, &read_fds)) { // if we have a socket ready
          // check to see if it is a client or the server
          // if it is the server, it means we have a new connection to accept
          if (socket == server_socket) {
            memset(&client_address, 0, sizeof(struct sockaddr_in));
            new_client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
            if (new_client_socket == -1) {
              perror("Error on accepting a new connection");
              return 2;
            }
            FD_SET(new_client_socket, &master); // add the new client connection to the master set
            if (new_client_socket > fdmax) {
              fdmax = new_client_socket; // keep track of the maximum file descriptor value(is used in the for loop)
            }
            printf("New connection from: %s on port %d\n", inet_ntoa(client_address.sin_addr), htons(client_address.sin_port));
          } else {
            // if it is a client socket, it means we need to read from it
            // read from the client socket
            // here is the logic for the client socket handling
            uint32_t recvNum;
            int noRecvBytes = recv(socket, &recvNum, sizeof(uint32_t), 0);
            if (noRecvBytes <= 0) {
              // got error or connection closed by client
              if (noRecvBytes == 0) {
                // connection closed
                printf("Connection closed: socket %d hung up\n", socket);
              } else {
                perror("Error on recv()");
                return 3;
              }
              close(socket);           // bye!
              FD_CLR(socket, &master); // remove from master set
            } else {
              // here is the logic of the program that handles the data recieved from the client
              // take the value recieved and convert it from big-endian representation
              float CRF = ntohf(recvNum); // floats are on 32 bits so its the same size as a long type
              float currDif;
              if (SRF - CRF >= 0) {
                currDif = SRF - CRF;
              } else {
                currDif = CRF - SRF;
              }
              if (currDif < min_dif) {
                winner_client = socket;
                min_dif = currDif;
              }
            }
          }
        }
      }
    }
  }

  // This point in code is never reached due to the infinite loop
  return 0;
}
