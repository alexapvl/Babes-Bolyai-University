#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int socket_fd;

void tratare(char* path) {
  int code;
  const char* file = "text.txt";
  char absolute_path[4096];

  // if (access(file, F_OK) == 0) {
  //   if (realpath(file, absolute_path) == NULL) {
  //     fprintf(stderr, "Could not find absolute path of file\n");
  //   }
  // } else {
  //   fprintf(stderr, "Could not find file\n");
  //   return;
  // }

  strcpy(absolute_path, path);
  printf("%s\n", absolute_path);
  int32_t path_length = strlen(absolute_path);
  path_length = htons(path_length);

  code = send(socket_fd, &path_length, sizeof(int32_t), 0);
  if (code < 0) {
    fprintf(stderr, "Could not send path length\n");
    return;
  }

  code = send(socket_fd, absolute_path, strlen(absolute_path), 0);
  if (code < 0) {
    fprintf(stderr, "Could not send path\n");
    return;
  }

  int32_t buffer_length;
  char result[4000 * 10];
  char buffer[4000];

  while (1) {
    code = recv(socket_fd, &buffer_length, sizeof(int32_t), 0);
    if (code < 0) {
      fprintf(stderr, "Error receiving buffer length\n");
      return;
    }
    buffer_length = ntohs(buffer_length);

    if (buffer_length == 0 || buffer_length == -1)
      break;

    code = recv(socket_fd, buffer, buffer_length, 0);
    if (code < 0) {
      fprintf(stderr, "Error receiving buffer\n");
      return;
    }

    strcat(result, buffer);
  }

  if (buffer_length != -1)
    printf("\nResult:\n%s\n", result);

  char file_copy[100];
  strcat(file_copy, file);
  strcat(file_copy, "-copy");

  printf("file name: %s\n", file_copy);
  int32_t total_length = strlen(result);

  FILE* fd = fopen(file_copy, "w");
  code = fwrite(result, 1, total_length, fd);
  if (code < 0) {
    fprintf(stderr, "Error writing into copy file\n");
    return;
  }

  fclose(fd);
}

int main(int argc, char** argv) {
  struct sockaddr_in server;
  int code;

  printf("Argv 1: %s\n", argv[1]);

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd < 0) {
    fprintf(stderr, "Error creating client socket\n");
    return 1;
  }

  memset(&server, 0, sizeof(struct sockaddr_in));

  server.sin_family = AF_INET;
  server.sin_port = htons(8000);
  server.sin_addr.s_addr = inet_addr("172.20.109.61");

  code = connect(socket_fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in));

  if (code < 0) {
    fprintf(stderr, "Error connecting to server socket\n");
    return 1;
  }

  // do stuff here

  tratare(argv[1]);

  close(socket_fd);
}