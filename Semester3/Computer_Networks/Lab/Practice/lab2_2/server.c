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
#include <sys/wait.h>
#include <unistd.h>

int client_fd;

void tratare() {
  int32_t path_length = 100;
  int code;
  code = recv(client_fd, &path_length, sizeof(int32_t), 0);
  if (code < 0) {
    fprintf(stderr, "Error receiving path length\n");
    return;
  }
  path_length = ntohs(path_length);
  printf("Path length: %d\n", path_length);

  char path[path_length];

  code = recv(client_fd, &path, path_length, 0);
  if (code < 0) {
    fprintf(stderr, "Error receiving path\n");
    return;
  }
  printf("Path: %s\n", path);

  int32_t buffer_length;
  // memset(buffer, 0, 4000);

  FILE* file = fopen(path, "r");
  if (file == NULL) { // file doesn't exist / path doesn't exist
    buffer_length = -1;
    printf("minus one, no file found %d\n", buffer_length);
    buffer_length = htons(buffer_length);

    code = send(client_fd, &buffer_length, sizeof(int32_t), 0);
    if (code < 0) {
      fprintf(stderr, "Could not send buffer length\n");
      return;
    }
  }

  char buffer[4000];
  while (fread(buffer, 1, 2000, file) != 0) {
    buffer_length = strlen(buffer);
    printf("buffer length %d\n", buffer_length);
    buffer_length = htons(buffer_length);

    code = send(client_fd, &buffer_length, sizeof(int32_t), 0);
    if (code < 0) {
      fprintf(stderr, "Could not send buffer length\n");
      return;
    }

    // memset(buffer, 0, 4000);
    code = send(client_fd, buffer, strlen(buffer), 0);
    if (code < 0) {
      fprintf(stderr, "Could not send buffer\n");
      return;
    }
    printf("%s\n", buffer);
  }

  int32_t zero = 0;
  code = send(client_fd, &zero, sizeof(int32_t), 0);
  if (code < 0) {
    fprintf(stderr, "Could not send zero\n");
    return;
  }

  exit(0);
}

int main() {
  struct sockaddr_in client, server;
  int socket_fd;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&server, 0, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = htons(8000);
  server.sin_addr.s_addr = INADDR_ANY;

  int code;

  code = bind(socket_fd, (const struct sockaddr*)&server, sizeof(struct sockaddr_in));

  if (code < 0) {
    fprintf(stderr, "Could not bind server structure to socket\n");
    return 1;
  }

  listen(socket_fd, 5);

  while (1) {
    memset(&client, 0, sizeof(struct sockaddr_in));
    int l = sizeof(struct sockaddr_in);

    printf("Waiting on clients..\n");
    client_fd = accept(socket_fd, (struct sockaddr*)&client, &l);

    if (client_fd < 0) {
      fprintf(stderr, "Could not accept client connection\n");
      return 1;
    }

    printf("Client connected with address %s and port %d.\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    if (fork() == 0) {
      tratare();
    }
  }

  wait(0);

  return 0;
}