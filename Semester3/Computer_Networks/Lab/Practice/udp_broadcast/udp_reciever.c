#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 1024 // Buffer length

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <Port>\n", argv[0]);
    return 1;
  }

  int reciever_socket;
  struct sockaddr_in server_address;
  char buffer[BUFLEN];

  // Create a UDP socket
  reciever_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (reciever_socket < 0) {
    perror("Socket creation error:");
    return 1;
  }

  // Setup the server address struct
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(argv[1]));
  server_address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

  // Bind the socket
  if (bind(reciever_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    perror("Binding error:");
    close(reciever_socket);
    return 1;
  }

  struct sockaddr_in sender_address;
  socklen_t sender_address_size = sizeof(sender_address);

  printf("Listening for broadcast messages...\n");
  while (1) {
    // Clear the buffer
    memset(buffer, 0, BUFLEN);

    // Receive a message
    ssize_t recv_len = recvfrom(reciever_socket, buffer, BUFLEN - 1, 0, (struct sockaddr*)&sender_address, &sender_address_size);
    if (recv_len < 0) {
      perror("Receive error:");
      continue; // Move to the next iteration in case of error
    }

    buffer[recv_len] = '\0'; // Null-terminate the message

    // Print the received message and sender information
    printf("Received message from %s:%d -> %s\n", inet_ntoa(sender_address.sin_addr), ntohs(sender_address.sin_port), buffer);
  }

  // Close the socket
  close(reciever_socket);
  return 0;
}
