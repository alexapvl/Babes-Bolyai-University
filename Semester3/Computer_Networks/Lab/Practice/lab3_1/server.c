#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 1024 // Buffer length

typedef struct {
  int socket;
  char buffer[BUFLEN];
  struct sockaddr_in address;
} ThreadArgs;

void* handle_client(void* args) {
  char buffer[BUFLEN];
  ThreadArgs* thread_args = (ThreadArgs*)args;
  struct sockaddr_in sender_address = thread_args->address;
  int client_socket = thread_args->socket;
  socklen_t sender_address_size = sizeof(sender_address);

  while (1) {
    // Clear the buffer
    memset(buffer, 0, BUFLEN);
    // Receive a message
    ssize_t recv_len = recvfrom(client_socket, buffer, BUFLEN - 1, 0, (struct sockaddr*)&sender_address, &sender_address_size);
    if (recv_len < 0) {
      perror("Receive error:");
      break;
    }
    buffer[recv_len] = '\0'; // Null-terminate the message
    // Print the received message and sender information
    printf("Received message from %s:%d -> %s\n", inet_ntoa(sender_address.sin_addr), ntohs(sender_address.sin_port), buffer);
    printf("Now sending it back...\n");
    if (sendto(client_socket, buffer, strlen(buffer), 0, (struct sockaddr*)&sender_address, sender_address_size) < 0) {
      perror("Send error:");
      break;
    }
  }
  free(args);
  return NULL;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: %s <Port>\n", argv[0]);
    return 1;
  }

  int receiver_socket;
  pthread_t thread_id;
  struct sockaddr_in server_address;

  // Create a UDP socket
  receiver_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (receiver_socket < 0) {
    perror("Socket creation error:");
    return 1;
  }

  // Setup the server address struct
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(argv[1]));
  server_address.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces

  // Bind the socket
  if (bind(receiver_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    perror("Binding error:");
    close(receiver_socket);
    return 1;
  }

  printf("Listening for broadcast messages...\n");

  ThreadArgs* thread_args = malloc(sizeof(ThreadArgs));

  while (1) {
    // Prepare for a new client
    memset(thread_args, 0, sizeof(ThreadArgs));
    thread_args->socket = receiver_socket;

    struct sockaddr_in sender_address;
    socklen_t sender_address_size = sizeof(sender_address);
    ssize_t recv_len = recvfrom(receiver_socket, thread_args->buffer, BUFLEN - 1, 0, (struct sockaddr*)&sender_address, &sender_address_size);
    if (recv_len < 0) {
      perror("Receive error:");
      continue; // Move to the next iteration in case of error
    }

    thread_args->buffer[recv_len] = '\0'; // Null-terminate the message
    thread_args->address = sender_address;

    // Create a new thread for each received message
    if (pthread_create(&thread_id, NULL, handle_client, thread_args) != 0) {
      perror("Failed to create thread");
      continue;
    }
    // Detach the thread to reclaim resources once done
    pthread_detach(thread_id);
  }

  // Close the socket
  close(receiver_socket);
  return 0;
}
