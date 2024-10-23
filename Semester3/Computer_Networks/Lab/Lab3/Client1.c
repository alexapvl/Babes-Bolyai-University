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

  socket_file_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_file_desc < 0) {
    printf("Error when creating the client socket\n");
    return 1;
  }

  memset(&server, 0, sizeof(server));
  server.sin_port = htons(8003);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("172.30.253.213");

  if (connect(socket_file_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
    printf("Error when connecting to the server\n");
    return 1;
  }

  // Get input string
  char *string_to_send = NULL;
  size_t buffer_size = 0;
  printf("String: ");
  getline(&string_to_send, &buffer_size, stdin);
  
  // Remove newline character from input string if it exists
  size_t length = strlen(string_to_send);
  if (length > 0 && string_to_send[length - 1] == '\n') {
      string_to_send[length - 1] = '\0';
      --length;
  }

  size_t converted_length = htonl(length);

  if(send(socket_file_desc, &converted_length, sizeof(size_t), 0) < 0) {
    printf("Error when sending the length\n");
    close(socket_file_desc);
    return 1;
  }

  if(send(socket_file_desc, string_to_send, length, 0) < 0) {
    printf("Error when sending string\n");
    close(socket_file_desc);
    return 1;
  }

  char string_to_recieve[100];

  size_t i = 0;
  while(i < length) {
    char letter;
    int cod = recv(socket_file_desc, &letter, 1, 0);
    if (cod < 1) {
      printf("Error at recieving letter %zu\n", i);
      return 1;
    }
    string_to_recieve[i] = letter;

    ++i;
  }
  string_to_recieve[i] = '\0';

  size_t length_recieved = strlen(string_to_recieve);

  printf("Recieved string is:\n%s\nThe length is: %zu\n", string_to_recieve, length_recieved);

  close(socket_file_desc);

  return 0;
}