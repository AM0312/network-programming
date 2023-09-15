#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>

#define MAX 80
#define PORT 8080
#define sa struct sockaddr

void chat_func(int sockfd)
{
    struct sockaddr_in cli_addr;
    int len = sizeof(cli_addr);
    int n;
    char b1[MAX], b2[MAX];
    for (;;)
    {
        bzero(&b1, sizeof(b1));
        bzero(&b2, sizeof(b2));
        if (recvfrom(sockfd, &b1, sizeof(b1), 0, (sa *)&cli_addr, &len) < 0)
        {
            perror("Couldn't receive from Client");
            exit(EXIT_FAILURE);
        }
        else
        {
            if (strncmp("exit", b1, 4) == 0)
            {
                printf("Client exited, server also exiting!");
                break;
            }
            printf("Client texted : %s\n", b1);
        }
        printf("Server message : ");
        fgets(b2, sizeof(b2), stdin);
        if (sendto(sockfd, &b2, sizeof(b2), 0, (sa *)&cli_addr, len) < 0)
        {
            perror("Unable to send message to Client!\n");
            exit(EXIT_FAILURE);
        }
    }
    return;
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Socket not created!\n");
        exit(EXIT_FAILURE);
    }
    printf("UDP Server Socket created!\n");
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (sa *)&serv_addr, sizeof(serv_addr)) != 0)
    {
        perror("Socket unable to bind!");
        exit(EXIT_FAILURE);
    }

    chat_func(sockfd);
    close(sockfd);
    return 0;
}