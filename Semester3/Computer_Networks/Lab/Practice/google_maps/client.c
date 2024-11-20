#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define SERVER_PORT 12345
#define BUFFER_SIZE 1024
#define MAX_MESSAGE_LENGTH 1024

int sockfd;
struct sockaddr_in server_addr;

void *receive_messages(void *arg) {
    while (1) {

        // we receive n and m fro server
        // we receive the map from server and we print it
        uint16_t m, n;
        ssize_t bytes_received = recv(sockfd, &m, sizeof(m), 0);
        m = ntohs(m);
        if(bytes_received < 0){
            exit(1);
        }
        bytes_received = recv(sockfd, &n, sizeof(n), 0);
        n = ntohs(n);
        if(bytes_received < 0){
            exit(1);
        }
        printf("m=%hu and n = %hu\n", m, n);
        int i = 1;
        char c;
        while(i <= m * n) {
            if(recv(sockfd, &c, 1,0) < 0)
                exit(1);
            printf("%s ",&c);
            if(i % n == 0)
                printf("\n");
            i ++;
        }
    }
    return NULL;
}

void send_message() {

    while (1) {
        uint16_t x,y;
        scanf("%hu", &x);
        scanf("%hu", &y);

        x = htons(x);
        if (send(sockfd, &x, sizeof(x), 0) < 0) {
            perror("Failed to send message");
            break;
        }
        y = htons(y);
        if (send(sockfd, &y, sizeof(y), 0) < 0) {
            perror("Failed to send message");
            break;
        }
    }
}

int main() {
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("172.20.10.6");  // Server address
    server_addr.sin_port = htons(SERVER_PORT);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server. You can start sending messages.\n");

    // Start the receive thread
    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, receive_messages, NULL) != 0) {
        perror("Failed to create receive thread");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Send messages in the main thread
    send_message();

    // Wait for the receive thread to finish
    pthread_join(receive_thread, NULL);

    close(sockfd);
    return 0;
}
