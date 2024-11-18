#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Function to convert float to a network byte order
uint32_t htonf(float f) {
  uint32_t p;
  // reinterpret float bits as int bits
  memcpy(&p, &f, sizeof(f));
  // convert to network byte order
  return htonl(p);
}

float generateRandomFloat(float a) {
  srand((unsigned int)clock());
  return ((float)rand() / (float)RAND_MAX) * a;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: %s, <IPAddress> <Port>\n", argv[0]);
    return 1;
  }

  int client_socket;
  struct sockaddr_in server_address;

  // Create a socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket < 0) {
    perror("Socket creation error:");
    return 2;
  }

  // Setup the server address struct
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_port = htons(atoi(argv[2])); // Set server port
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(argv[1]); // Server IP address

  // Connect to the server
  if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    perror("Connection error:");
    return 3;
  }

  // **Data Exchange Logic Placeholder**
  // -----------------------------
  // To send data:
  // Use `send(client_socket, &buffer, sizeof(buffer), 0);`
  // where `buffer` is the data you want to send.
  float randomNumber = generateRandomFloat(10.0);
  printf("I generated the number %f\n", randomNumber);
  uint32_t number = htonf(randomNumber);                         // convert to big-endian
  if (send(client_socket, &number, sizeof(uint32_t), 0) == -1) { // send number to server
    close(client_socket);
    perror("Error on send()");
    return 4;
  }

  // To receive data:
  // Use `recv(client_socket, &buffer, sizeof(buffer), 0);`
  // where `buffer` is where the incoming data will be stored.
  // -----------------------------

  size_t messageLen;
  char message[100];
  recv(client_socket, &messageLen, sizeof(size_t), 0);
  messageLen = ntohs(messageLen);
  recv(client_socket, &message, messageLen, 0);
  printf("%s", message);

  // Close the socket
  close(client_socket);

  return 0;
}
