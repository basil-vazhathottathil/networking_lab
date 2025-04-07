#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT 8080
#define MAX 80

int main() {
    int sockfd;
    char buff[MAX];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    printf("UDP Server listening on port %d...\n", PORT);

    while (1) {
        memset(buff, 0, MAX);
        len = sizeof(cliaddr);
        recvfrom(sockfd, buff, MAX, 0, (struct sockaddr *)&cliaddr, &len);
        printf("Client: %s", buff);

        if (strncmp(buff, "exit", 4) == 0) {
            printf("Server exiting...\n");
            break;
        }

        printf("Server: ");
        memset(buff, 0, MAX);
        int n = 0;
        while ((buff[n++] = getchar()) != '\n');

        sendto(sockfd, buff, strlen(buff), 0, (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}
