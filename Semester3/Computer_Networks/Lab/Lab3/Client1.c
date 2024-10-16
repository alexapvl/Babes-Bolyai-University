#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
  int socket_file_desc;
  struct sockaddr_in server;
  uint16_t a, b, suma;

  socket_file_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_file_desc < 0) {
    printf("Error when creating the client socket\n");
    return 1;
  }

  memset(&server, 0, sizeof(server));
  server.sin_port = htons(3002);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("172.30.245.235");

  if (connect(socket_file_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
    printf("Error when connecting to the server\n");
    return 1;
  }

  char* string_to_send;
  size_t string_length;

  printf("String: ");
  getline(&string_to_send, &string_length, stdin);

  size_t converted_length = htonl(string_length);
  send(socket_file_desc, &converted_length, sizeof(converted_length), 0);

  for(int i = 0; i < string_length; i++) {
    send(socket_file_desc, string_to_send + i, sizeof(char), 0);
  }

  close(socket_file_desc);

  return 0;
}