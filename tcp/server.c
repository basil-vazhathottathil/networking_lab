#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX 80

void chat(int connfd) {
    char buff[MAX];
    int n;

    while (1) {
        memset(buff, 0, MAX);
        read(connfd, buff, sizeof(buff));
        printf("Client: %s", buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("Server exiting...\n");
            break;
        }

        printf("Server: ");
        memset(buff, 0, MAX);
        n = 0;
        while ((buff[n++] = getchar()) != '\n');

        write(connfd, buff, strlen(buff));
    }
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    memset(&servaddr, 0, sizeof(servaddr));

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(sockfd, 5);
    printf("Server listening on port %d...\n", PORT);

    len = sizeof(cli);
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    if (connfd < 0) {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected.\n");
    chat(connfd);
    close(sockfd);
    return 0;
}
