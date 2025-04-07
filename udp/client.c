#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 80

int main() {
    int sockfd;
    char buff[MAX];
    struct sockaddr_in servaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Connected to server (UDP)...\n");

    while (1) {
        memset(buff, 0, MAX);
        printf("Client: ");
        int n = 0;
        while ((buff[n++] = getchar()) != '\n');

        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

        memset(buff, 0, MAX);
        len = sizeof(servaddr);
        recvfrom(sockfd, buff, MAX, 0, (struct sockaddr *)&servaddr, &len);
        printf("Server: %s", buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client exiting...\n");
            break;
        }
    }

    close(sockfd);
    return 0;
}
