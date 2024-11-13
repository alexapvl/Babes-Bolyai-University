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

void tratare(int argc, char* argv[]) {
  int code;

  if (argc < 2) {
    fprintf(stderr, "Insufficient arguments\n");
    return;
  }

  char command[100];
  strcpy(command, argv[1]);

  int32_t command_size = strlen(command);
  command_size = htons(command_size);
  code = send(socket_fd, &command_size, sizeof(int32_t), 0);

  if (code < 0) {
    fprintf(stderr, "Error sending command length\n");
    return;
  }

  code = send(socket_fd, command, command_size, 0);

  if (code < 0) {
    fprintf(stderr, "Error sending command\n");
    return;
  }

  int32_t buffer_size;
  char buffer[1024];
  //  char result[1024 * 10] = {0};

  while (1) {
    // receive first how much and then.. stop when u receive 0
    code = recv(socket_fd, &buffer_size, sizeof(buffer_size), 0);
    if (code < 0) {
      fprintf(stderr, "Error receiving buffer size\n");
      return;
    }
    buffer_size = ntohl(buffer_size);
    printf("received buffer size %d\n", buffer_size);

    if (buffer_size == 0) {
      break;
    }

    memset(buffer, 0, 1024);
    code = recv(socket_fd, buffer, buffer_size, 0);
    if (code < 0) {
      fprintf(stderr, "Error receiving buffer\n");
      return;
    }
    buffer[buffer_size] = '\0';
    printf("received buffer %s\n", buffer);

    // strcat(result, buffer);
    printf("%s", buffer);
  }

  // printf("%s\n", result);
  int32_t exit_code;

  code = recv(socket_fd, &exit_code, sizeof(int32_t), 0);
  if (code < 0) {
    fprintf(stderr, "Error receiving buffer size\n");
    return;
  }
  exit_code = ntohl(exit_code);

  printf("Exit code is: %d\n", exit_code);

  return;
}

int main(int argc, char* argv[]) {
  struct sockaddr_in server;
  int code;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd < 0) {
    fprintf(stderr, "Error creating client socket\n");
    return 1;
  }

  memset(&server, 0, sizeof(struct sockaddr_in));

  server.sin_family = AF_INET;
  server.sin_port = htons(8001);
  server.sin_addr.s_addr = inet_addr("172.30.247.175");

  code = connect(socket_fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in));

  if (code < 0) {
    fprintf(stderr, "Error connecting to server socket\n");
    return 1;
  }

  // do stuff here
  tratare(argc, argv);

  close(socket_fd);
  return 0;
}