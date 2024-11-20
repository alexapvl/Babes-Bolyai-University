#include <sys/types.h>

#include <sys/socket.h>

#include <stdio.h>

#include <netinet/in.h>

#include <netinet/ip.h>

#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>


int main() {
    int c;
    struct sockaddr_in server;
    long lens;
    char *s;

    c = socket(AF_INET, SOCK_STREAM, 0);
    if(c < 0) {
        printf("Eroare la crearea socketului client\n");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(1234);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("172.30.243.12");

    if(connect(c, (struct sockaddre *) &server, sizeof(server)) < 0) {
        printf("Eroare la conectarea la server\n");
        return 1;
    }

    FILE *file = fopen("input.txt", "r");
    if(file == NULL) {
        printf("Eroare la deschiderea fisierului\n");
        close(c);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file,0,SEEK_SET);

    s = (char*)malloc(file_size + 1);
    if(s == NULL) {
        perror("Memory allocation error");
        fclose(file);
        close(c);
        return 1;
    }
    fread(s,1,file_size,file);
    s[file_size] = '\0';
    fclose(file);
    lens = file_size;

    lens = htonl(lens);
    int nrc = send(c, &lens, sizeof(lens), 0);
    if(nrc != sizeof(lens)) {
        printf("Error sending string length");
        free(s);
        close(c);
        return 1;
    }

    if(send(c,s,file_size,0) != file_size) {
        printf("Error sending string");
        free(s);
        close(c);
        return 1;
    }

    char* news = (char*)malloc(file_size + 1);
    if(news == NULL) {
        printf("Memory allocation failed\n");
        close(c);
        exit(2);
    }

    // Initialize variables for receiving in chunks
    int received = 0;
    int rs;
    while(received < file_size) {
        // Calculate remaining bytes
        int bytes_to_receive = file_size - received;
        
        // Receive data in chunks
        rs = recv(c, news + received, bytes_to_receive, 0);
        if (rs < 0) {
            printf("Error receiving data\n");
            free(s);
            close(c);
            exit(3);
        }
        received += rs;
    }
    
    printf("\nThe length of the string is: %hu\n", strlen(news) - 1);
    printf("The reversed string is: %s\n", news);
    free(s);
    close(c);
}