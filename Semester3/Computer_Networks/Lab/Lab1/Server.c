#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <stdio.h>

#ifndef _WIN32
// Non-Windows headers
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define closesocket close
typedef int SOCKET;
#else
// Windows headers
#include <WinSock2.h>
#include <cstdint>

// Uncomment the pragma if you want to link the library directly
// #pragma comment(lib, "Ws2_32.lib")
#endif

int main() {
  SOCKET s;
  struct sockaddr_in server, client;
  int client_socket, err;
  socklen_t client_len;

#ifdef _WIN32
  // Initialize Windows Sockets Library
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {
    printf("Error initializing Windows Sockets Library\n");
    return -1;
  }
#endif

  // Create server socket
  s = socket(AF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    printf("Error creating server socket\n");
    return 1;
  }

  // Set up server address structure
  memset(&server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(1234);
  server.sin_addr.s_addr = INADDR_ANY;

  // Bind the socket to the address
  if (bind(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
    perror("Bind error");
    return 1;
  }

  // Start listening for connections
  listen(s, 5);
  client_len = sizeof(client);
  memset(&client, 0, sizeof(client));

  while (1) {
    uint16_t a, b, sum;

    printf("Listening for incoming connections...\n");

    // Accept client connection
    client_socket = accept(s, (struct sockaddr*)&client, &client_len);
    err = errno;
#ifdef _WIN32
    err = WSAGetLastError();
#endif
    if (client_socket < 0) {
      printf("Accept error: %d\n", err);
      continue;
    }

    printf("Incoming connection from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    // Receive the two operands
    if (recv(client_socket, (char*)&a, sizeof(a), 0) != sizeof(a)) {
      printf("Error receiving first operand\n");
      closesocket(client_socket);
      continue;
    }

    if (recv(client_socket, (char*)&b, sizeof(b), 0) != sizeof(b)) {
      printf("Error receiving second operand\n");
      closesocket(client_socket);
      continue;
    }

    // Convert from network byte order to host byte order
    a = ntohs(a);
    b = ntohs(b);

    // Compute the sum and convert back to network byte order
    sum = htons(a + b);

    // Send the result back to the client
    if (send(client_socket, (char*)&sum, sizeof(sum), 0) != sizeof(sum)) {
      printf("Error sending result\n");
    }

    // Close client connection
    closesocket(client_socket);
  }

#ifdef _WIN32
  // Cleanup Windows Sockets Library
  WSACleanup();
#endif

  return 0;
}
