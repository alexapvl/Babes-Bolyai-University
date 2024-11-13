// The client sends an array of boolean values. The server counts how many true values are in the array and returns the count.
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int client_socket;
  struct sockaddr_in server_address;

  // Create a socket
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket < 0) {
    perror("Socket creation error:");
    return 1;
  }

  // Setup the server address struct
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_port = htons(8000); // Set server port
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr("172.20.10.9"); // Server IP address

  // Connect to the server
  if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    perror("Connection error:");
    return 1;
  }

  // **Data Exchange Logic Placeholder**
  // -----------------------------
  // To send data:
  // Use `send(client_socket, &buffer, sizeof(buffer), 0);`
  // where `buffer` is the data you want to send.

  uint16_t arrLen = 0;
  printf("Input the length of the array: ");
  scanf("%hd", &arrLen);
  uint16_t copyArrLen = arrLen;
  arrLen = htons(arrLen);
  // printf("Arr length is: %d\n", arrLen);

  uint16_t boolArr[arrLen];
  for (int i = 0; i < copyArrLen; i++) {
    printf("True(1) or false(0)? Input: ");
    scanf("%hd", boolArr + i);
    boolArr[i] = htons(boolArr[i]); // convert to corrent byte order for network
  }

  // for (int i = 0; i < arrLen; i++) {
  //   printf("Index %d -> %d\n", i, boolArr[i]);
  // }

  if (send(client_socket, &arrLen, sizeof(uint16_t), 0) == -1) {
    perror("Error on sending the length of the array\n");
    return 1;
  }
  if (send(client_socket, &boolArr, copyArrLen * sizeof(uint16_t), 0) == -1) {
    perror("Error on sending the array of boolean values\n");
    return 1;
  }

  // To receive data:
  // Use `recv(client_socket, &buffer, sizeof(buffer), 0);`
  // where `buffer` is where the incoming data will be stored.
  // -----------------------------
  uint16_t result = 0;
  if (recv(client_socket, &result, sizeof(uint16_t), 0) == -1) {
    perror("Error on recieving the result from the server");
    return 1;
  }

  result = ntohs(result);
  printf("The number of truth values from the sent boolean array is: %d\n", result);

  // Close the socket
  close(client_socket);

  return 0;
}
