/* UDP client in the internet domain */
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void error(char*);

int main(int argc, char* argv[]) {
  int sock, n;
  socklen_t length;
  struct sockaddr_in server, from; // IP Addressing(ip, port, type) Stuff
  struct hostent* hp;              // DNS stuff
  char buffer[256];

  if (argc != 3) {
    printf("Usage: %s <server_name> <port>\n", argv[0]);
    exit(1);
  }

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0)
    error("socket");

  server.sin_family = AF_INET;
  hp = gethostbyname(argv[1]);

  if (hp == 0)
    error("Unknown host");

  // bcopy((char*)hp->h_addr, (char*)&server.sin_addr, hp->h_length);
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(atoi(argv[2]));
  length = sizeof(struct sockaddr_in);
  printf("Please enter the message: ");
  bzero(buffer, 256);
  fgets(buffer, 255, stdin);
  n = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server, length);

  if (n < 0)
    error("Sendto");

  n = recvfrom(sock, buffer, 256, 0, (struct sockaddr*)&from, &length);
  if (n < 0)
    error("recvfrom");

  write(1, "Got an ack: ", 12);
  write(1, buffer, n);
}

void error(char* msg) {
  perror(msg);
  exit(0);
}