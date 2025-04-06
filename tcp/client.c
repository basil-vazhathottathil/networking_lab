#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 80

void chat(int sockfd) {
    char buff[MAX];
    int n;

    while (1) {
        memset(buff, 0, MAX);
        printf("Client: ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n');

        write(sockfd, buff, strlen(buff));

        memset(buff, 0, MAX);
        read(sockfd, buff, sizeof(buff));
        printf("Server: %s", buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client exiting...\n");
            break;
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&servaddr, 0, sizeof(servaddr));

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server.\n");
    chat(sockfd);
    close(sockfd);
    return 0;
}
