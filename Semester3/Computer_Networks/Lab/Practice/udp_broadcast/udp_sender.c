#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: %s <Broadcast address> <Port>\n", argv[0]);
    return 1;
  }

  int broadcast_socket;
  struct sockaddr_in broadcast_address;
  char* message = "Hello, Network!";

  // Create a datagram socket
  broadcast_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (broadcast_socket < 0) {
    perror("Socket creation error:");
    return 1;
  }

  // Enable the broadcast option on the socket
  int enableBroadcast = 1;
  if (setsockopt(broadcast_socket, SOL_SOCKET, SO_BROADCAST, &enableBroadcast, sizeof(int)) < 0) {
    perror("Set broadcast option error:");
    close(broadcast_socket);
    return 1;
  }

  // Setup the broadcast address struct without binding to a specific port
  memset(&broadcast_address, 0, sizeof(broadcast_address));
  broadcast_address.sin_port = htons(atoi(argv[2])); // Destination port
  broadcast_address.sin_family = AF_INET;
  if (inet_aton(argv[1], &broadcast_address.sin_addr) == 0) {
    fprintf(stderr, "Invalid broadcast address\n");
    close(broadcast_socket);
    return 1;
  }

  // Send the message
  if (sendto(broadcast_socket, message, strlen(message), 0, (struct sockaddr*)&broadcast_address, sizeof(broadcast_address)) < 0) {
    perror("Sendto error:");
    close(broadcast_socket);
    return 1;
  }

  close(broadcast_socket);
  return 0;
}
