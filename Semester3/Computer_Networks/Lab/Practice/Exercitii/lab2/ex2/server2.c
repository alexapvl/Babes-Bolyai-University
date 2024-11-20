#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <fcntl.h>

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

void tratare() {
    char *filename = NULL;
    int filename_len = 0;
    int received_bytes = 0;
    char buffer[1024];

    // Receive filename length from client
    if(recv(c, &filename_len, sizeof(int32_t), 0) != sizeof(int32_t)) {
        printf("Error receiving the filename length");
        close(c);
        exit(1);
    }
    filename_len = ntohl(filename_len);
    printf("%d", filename_len);

    filename = (char *)malloc(filename_len + 1);
    if (filename == NULL) {
        perror("Memory allocation error for filename");
        close(c);
        exit(1);
    }

    memset(filename, 0, filename_len + 1);

    // Receive the filename
    while (received_bytes < filename_len) {
        int chunk = recv(c, buffer, sizeof(buffer), 0);
        if (chunk <= 0) {
            perror("Error reading filename");
            free(filename);
            close(c);
            exit(1);
        }
        strncat(filename, buffer, chunk);
        received_bytes += chunk;
    }

    if (filename[filename_len - 1] == '\n') {
        filename[filename_len - 1] = '\0';  // Null-terminate to remove the newline
        filename_len--;
    }
    

    // Open the file and send its length/content
    int file_len = -1;
    char *file_content = NULL;
    int fd = open(filename, O_RDONLY);
    if (fd != -1) {
        file_len = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);

        printf("%d", file_len);

        file_content = (char *)malloc(file_len + 1);
        if (file_content == NULL) {
            perror("Memory allocation error for file content");
            close(fd);
            free(filename);
            close(c);
            exit(1);
        }

        int bytes_read = read(fd, file_content, file_len);
        if (bytes_read != file_len) {
            perror("Error reading the file");
            free(file_content);
            close(fd);
            free(filename);
            close(c);
            exit(1);
        }

        close(fd);
    }

    file_len = htonl(file_len);
    if (send(c, &file_len, sizeof(int32_t), 0) != sizeof(int32_t)) {
        perror("Error sending file length");
        free(file_content);
        free(filename);
        close(c);
        exit(1);
    }

    if (file_len != -1) {
        if (send(c, file_content, ntohl(file_len), 0) != ntohl(file_len)) {
            perror("Error sending file content");
            free(file_content);
            free(filename);
            close(c);
            exit(1);
        }
        free(file_content);
    }

    free(filename);
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