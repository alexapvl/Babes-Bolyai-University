#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define closesocket close
typedef int SOCKET;

struct Person {
  char firstname[50], lastname[50];
  uint16_t age;
};

int main() {
  SOCKET server_socket;
  struct sockaddr_in server_address, client_address;
  int client_address_length;

  // Create a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("Socket creation error:");
    return 1;
  }

  // Setup server address struct
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_port = htons(8000); // Set port
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY; // Accept any incoming

  // Bind the socket
  if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    perror("Bind error:");
    return 1;
  }

  // Listen for incoming connections
  listen(server_socket, 5);

  // Initialize the client address struct length
  client_address_length = sizeof(client_address);
  memset(&client_address, 0, sizeof(client_address));

  // Main loop to accept and handle clients
  while (1) {
    printf("Listening for incoming connections...\n");

    int client_socket = accept(server_socket, (struct sockaddr*)&client_address, (socklen_t*)&client_address_length);
    if (client_socket < 0) {
      perror("Accept error:");
      continue;
    }

    // Handle connection with the client
    printf("Incoming connected client from: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    // Example placeholder for serving the connected client
    // Replace the following with actual code to handle client requests
    uint16_t sizeOfPerson = 0;
    if (recv(client_socket, &sizeOfPerson, sizeof(uint16_t), 0) == -1) {
      perror("Error when recieving the size of the person");
      return 1;
    }
    sizeOfPerson = ntohs(sizeOfPerson);

    printf("The person structure size is %hd bytes\n", sizeOfPerson);

    struct Person recievedPerson;
    if (recv(client_socket, &recievedPerson, sizeOfPerson, 0) == -1) {
      perror("Error on recieving the person structure");
      return 1;
    }
    recievedPerson.age = ntohs(recievedPerson.age);

    printf("Hello! I am %s %s and I am %hd years old, I will soon be %d.\n", recievedPerson.firstname, recievedPerson.lastname, recievedPerson.age, recievedPerson.age + 1);

    recievedPerson.age++; // increase the age
                          // send the structure back

    recievedPerson.age = htons(recievedPerson.age); // convert back the age to big endian representation
    if (send(client_socket, &recievedPerson, sizeof(struct Person), 0) == -1) {
      perror("Error when sending the structure back to the client");
      return 1;
    }

    closesocket(client_socket); // Close client socket
  }

  // This point in code is never reached due to the infinite loop
  // Insert necessary cleanup code if the program design changes

  return 0;
}
