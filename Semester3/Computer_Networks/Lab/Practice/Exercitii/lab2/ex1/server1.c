#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

int c;

// Mecanismul de time-out.
void time_out(int semnal) {
  int32_t r = -1;
  r = htonl(r);
  printf("Time out.\n");
  send(c, &r, sizeof(int32_t), 0);
  close(c); // inchidem conexiunea cu clientul
  exit(1);
}

// Function to execute command received from client and return output and exit status.
void execute_command(const char *command) {
    FILE *fp;
    char *output = NULL;
    size_t output_size = 0;
    size_t buffer_size = 1024;
    int32_t exit_code;

    // Allocate initial memory for output buffer
    output = (char *)malloc(buffer_size);
    if (output == NULL) {
        perror("Eroare la alocarea memoriei pentru output.");
        exit_code = -1;
        if(send(c, &exit_code, sizeof(int32_t), 0) != sizeof(int32_t))
            printf("Error sending the exit code");
        close(c);
        exit(1);
    }

    // Use popen to execute command and capture output
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Eroare la popen");
        free(output);
        exit_code = -1;
        if(send(c, &exit_code, sizeof(int32_t), 0) != sizeof(int32_t))
            printf("Error sending the exit code");
        close(c);
        exit(1);
    }

    // Read command output and reallocate if needed
    size_t total_bytes_read = 0;
    while (fgets(output + total_bytes_read, buffer_size - total_bytes_read, fp) != NULL) {
        total_bytes_read += strlen(output + total_bytes_read);
        if (total_bytes_read >= buffer_size - 1) { // If buffer is full, reallocate
            buffer_size *= 2;
            output = (char *)realloc(output, buffer_size);
            if (output == NULL) {
                perror("Eroare la realocarea memoriei pentru output.");
                pclose(fp);
                exit_code = -1;
                close(c);
                exit(1);
            }
        }
    }

    int nrbytes = htonl(total_bytes_read+1);

    printf("The output of the command is: %s \n",output);

    //send nr of bytes from output
    if(send(c,&nrbytes,sizeof(int),0) != sizeof(int)) {
        printf("Error sending the nr of bytes of the output");
        exit_code = -1;
    }

    output[strlen(output)] = '\0';
    // Send command output to client
    if(send(c, output, total_bytes_read + 1, 0) != total_bytes_read + 1) {
        printf("Error sending the output");
        exit_code = -1;
    }

    // Close popen stream and get exit code
    exit_code = pclose(fp);
    if (WIFEXITED(exit_code)) {
        exit_code = WEXITSTATUS(exit_code);
    } else {
        exit_code = -1;
    }
    printf("------%d", exit_code);
    exit_code = htonl(exit_code);
    if(send(c, &exit_code, sizeof(int32_t), 0) != sizeof(int32_t)) {
        printf("Error sending the exit code");
        free(output);
        close(c);
        exit(1);
    }

    // Clean up
    free(output);
    close(c);
    exit(0);
}

void tratare() {
    char *command = NULL;
    int command_len = 0;
    int received_bytes = 0;
    char buffer[1024];

    // Receive command length from client
    if(recv(c, &command_len, sizeof(int32_t), 0) != sizeof(int32_t)) {
        printf("Error receiving the command length");
        close(c);
        exit(1);
    }
    command_len = ntohl(command_len);

    printf("Length: %d \n",command_len);

    // Allocate memory for command string
    command = (char *)malloc(command_len + 1); // +1 for null terminator
    if (command == NULL) {
        perror("Eroare la alocarea memoriei pentru comanda.");
        close(c);
        exit(1);
    }

    memset(command, 0, command_len + 1);  // Initialize to zero

    // Receive command from client in chunks if necessary
    while (received_bytes < command_len) {
        int chunk = recv(c, buffer, sizeof(buffer), 0);
        if (chunk <= 0) {
            printf("Eroare la citirea comenzii de la client.\n");
            free(command);
            close(c);
            exit(1);
        }
        strncat(command, buffer, chunk);
        received_bytes += chunk;
    }
    printf("Received command: %s \n",command);
    // Execute the received command
    execute_command(command);
    free(command);
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
  
    while (1) { // Serve multiple clients
        memset(&client, 0, sizeof(client));
        l = sizeof(client);
        printf("Astept sa se conecteze un client.\n");
        c = accept(s, (struct sockaddr *) &client, &l);
        printf("S-a conectat clientul cu adresa %s si portul %d.\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
    
        if (fork() == 0) { // create a new process to handle each client
            close(s); // child does not need the listener
            tratare();
        }
        close(c); // parent does not need this connection
    }
}