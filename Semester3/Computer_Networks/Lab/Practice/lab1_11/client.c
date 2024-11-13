// Define a simple structure (e.g., a Person object with firstname, lastname, gender  and age).
// The client sends this structure to the server. The server increments the age and sends the updated structure back.
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct Person {
  char firstname[50], lastname[50];
  uint16_t age;
};

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
  struct Person person;
  printf("Input the first name of the person: ");
  scanf("%s", person.firstname);
  printf("Input the last name of the person: ");
  scanf("%s", person.lastname);
  printf("Input the age of the person: ");
  scanf("%hd", &person.age);
  printf("Hello! I am %s %s and I am %hd years old.\n", person.firstname, person.lastname, person.age);

  person.age = htons(person.age);
  // no need to convert the first name and last name since they are a sequence of characters, i.e. sequence of bytes, so the conversion will not do anything

  uint16_t sizeOfPerson = sizeof(struct Person);
  sizeOfPerson = htons(sizeOfPerson);
  if (send(client_socket, &sizeOfPerson, sizeof(uint16_t), 0) == -1) {
    perror("Error when sending the size of the person structure");
    return 1;
  }
  if (send(client_socket, &person, sizeof(struct Person), 0) == -1) {
    perror("Error when sending the structure");
    return 1;
  }

  // To receive data:
  // Use `recv(client_socket, &buffer, sizeof(buffer), 0);`
  // where `buffer` is where the incoming data will be stored.
  // -----------------------------

  if (recv(client_socket, &person, sizeOfPerson, 0) == -1) {
    perror("Error when recieving the updated structure from the server");
    return 1;
  }

  person.age = ntohs(person.age); // convert the age

  printf("Yay! My name is %s %s and today is my birthday! I turned %hd!\n", person.firstname, person.lastname, person.age);

  // Close the socket
  close(client_socket);

  return 0;
}
