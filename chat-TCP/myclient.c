#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <sys/types.h>

#define MAX 80
#define PORT 8080
#define sa struct sockaddr

void chat_func(int sockfd)
{
    char buff[MAX];
    int n;
    for (;;)
    {
        bzero(buff, sizeof(buff));
        printf("Type your message : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        if (strncmp("exit", buff, 4) == 0)
        {
            printf("Client Exit...\n");
            break;
        }
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("From Server : %s", buff);
    }
    return;
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, client_addr;

    if (sockfd == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully created for Client!\n");
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(sockfd, (sa *)&server_addr, sizeof(server_addr)) != 0)
    {
        perror("Couldn't connect to server!");
        exit(EXIT_FAILURE);
    }
    else
        printf("Connected to server! Start Messaging!\n");

    chat_func(sockfd);
    close(sockfd);
    return 0;
}