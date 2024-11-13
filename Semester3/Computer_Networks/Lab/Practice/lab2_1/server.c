// Problem 1 from lab 2
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
  int code;

  int32_t command_size;
  code = recv(client_fd, &command_size, sizeof(int32_t), 0);
  if (code < 0) {
    fprintf(stderr, "Error receiving command length\n");
    return;
  }

  command_size = ntohs(command_size);
  printf("Length received: %d\n", command_size);

  char command[100];
  code = recv(client_fd, command, command_size, 0);
  if (code < 0) {
    fprintf(stderr, "Error receiving command\n");
    return;
  }

  command[command_size] = '\0';
  printf("Command received: %s\n", command);

  FILE* fp;
  fp = popen(command, "r");

  if (fp < 0) {
    fprintf(stderr, "popen failed..\n");
    return;
  } else
    printf("popen success!\n");

  char buffer[1024];
  int32_t bytes_read;

  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    bytes_read = htonl(strlen(buffer));

    code = send(client_fd, &bytes_read, sizeof(int32_t), 0);
    if (code < 0) {
      fprintf(stderr, "Error sending buffer size\n");
      pclose(fp);
      return;
    }

    code = send(client_fd, buffer, strlen(buffer), 0);
    if (code < 0) {
      fprintf(stderr, "Error sending the command output buffer\n");
      pclose(fp);
      return;
    }
  }

  int32_t end = 0;
  end = htonl(end);
  code = send(client_fd, &end, sizeof(int32_t), 0);
  if (code < 0) {
    fprintf(stderr, "Error sending end marker\n");
    pclose(fp);
    return;
  }

  // int32_t exit_code = WEXITSTATUS(pclose(fp));
  // printf("exit code: %d\n", exit_code);
  // exit_code = htonl(exit_code);

  // code = send(client_fd, &exit_code, sizeof(int32_t), 0);
  if (code < 0) {
    fprintf(stderr, "Error sending exit code\n");
    pclose(fp);
    return;
  }

  pclose(fp);

  exit(0);
}

int main() {
  struct sockaddr_in client, server;
  int socket_fd;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  memset(&server, 0, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = htons(8001);
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
    socklen_t l = sizeof(struct sockaddr_in);

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