#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>

int c;
 
// Mecanismul de time-out. Paragraful 3.
void time_out(int semnal) {
  int32_t r = -1;
  r = htonl(r);
  printf("Time out.\n");
  send(c, &r, sizeof(int32_t), 0);
  close(c); // desi nu am primit nimic de la client in 10 secunde, inchidem civilizat conexiunea cu acesta
  exit(1);
}
 
void tratare() {
    int cod;
    char s[1005];
    int i = 0;
    struct sockaddr_in server;

    if (c < 0) {
        fprintf(stderr, "Eroare la stabilirea conexiunii cu clientul.\n");
        exit(1);
    } else {
        printf("Un nou client s-a conectat cu succes.\n");
    }

    // Read the string from the client
    do {
        cod = recv(c, &s[i], 1, 0);
        i += 1;
        printf("Am primit %d caractere. String: %s\n", cod, s);  // Print the string as it is being received

        if (cod == 1) // citire cu succes a unui caracter
            alarm(10);  // resetam timerul si asteptam din nou 10 secunde urmatorul caracter

        if (cod != 1) {
            exit(1);
            break;
        }
    }
    while (s[i - 1] != 0); // terminarea la caracterul NULL
    s[i - 1] = '\0';

    // Debugging: Print the full received string
    printf("Received string: %s\n", s);

    i = 0;
    int a = 0, b = 0;
    char op;
    printf("Starting to parse string.\n");

    // Parsing the first number (a)
    while(i < strlen(s) && isdigit(s[i])) {
        printf("Processing digit: %c\n", s[i]);
        a = a * 10 + (s[i] - '0');
        i += 1;
    }

    printf("Finished parsing a: %d\n", a);  

    if(i == strlen(s)) {
        printf("Error: Missing operator or second number.\n");
        exit(1);
    }
    op = s[i];
    printf("Operator found: %c\n", op);  

    if(strchr("+-*/", op) == NULL) {
        printf("Error: Invalid operator %c.\n", op);
        exit(1);
    }
    i += 1;

    while(i < strlen(s) && isdigit(s[i])) {
        printf("Processing digit for b: %c\n", s[i]);  
        b = b * 10 + (s[i] - '0');
        i += 1;
    }

    printf("a = %d; b = %d, op = %c\n", a, b, op); 

    int sol;
    if(op == '+') sol = a + b;
    else if(op == '*') sol = a * b;
    else if(op == '-') sol = a - b;
    else if(op == '/') {
        if (b == 0) {
            printf("Error: division by zero\n");
            exit(1);
        }
        sol = a / b;
    }

    char message[1005];
    strcpy(message, s);
    strcat(message, " is ");
    snprintf(s, sizeof(s), "%d", sol);

    srand(time(NULL));  

    int random_number = rand();

    if(random_number % 6 == 0)
        strcat(message, "banana");
    else if(random_number % 3 == 0)
        strcat(message, "-1000");
    else 
        strcat(message, s);
    message[strlen(message)] = '\0';

    cod = send(c, message, strlen(message) + 1, 0);
    if (cod != strlen(message) + 1) {
        fprintf(stderr, "Eroare la trimiterea datelor la server.\n");
        return 1;
    }
    
    close(c);
    exit(0);
}

             
int main() {
  int s, l, cod;
  struct sockaddr_in client, server;  
  
  s = socket(PF_INET, SOCK_STREAM, 0);
  if (s < 0) {
    fprintf(stderr, "Eroare la creare socket server.\n");
    return 1;
  }
  
  memset(&server, 0, sizeof(struct sockaddr_in));
  server.sin_family = AF_INET;
  server.sin_port = htons(4321);
  server.sin_addr.s_addr = INADDR_ANY;
  
  cod = bind(s, (struct sockaddr *) &server, sizeof(struct sockaddr_in));
  if (cod < 0) {
    fprintf(stderr, "Eroare la bind. Portul este deja folosit.\n");
    return 1;
  }
  
  listen(s, 5);
  
  while (1) { // deserveste oricati clienti
 
    memset(&client, 0, sizeof(client));
    l = sizeof(client);
 
    printf("Astept sa se conecteze un client.\n");
    c = accept(s, (struct sockaddr *) &client, &l);
    printf("S-a conectat clientul cu adresa %s si portul %d.\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    
    if (fork() == 0) { // server concurent, conexiunea va fi tratata de catre un proces fiu separat
      tratare();
    }
    // parintele continua bucla while asteptand un nou client
  }
  
}