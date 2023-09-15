#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>

#include <sys/types.h>
#include <sys/socket.h>

#define MAX 80
#define PORT 8080
#define sa struct sockaddr

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Unable to create socket!");
        exit(0);
    }
    else
        printf("Server socket created!\n");

    struct sockaddr_in serv_addr, cli_addr;
    int len = sizeof(cli_addr);
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    memset(&cli_addr, 0, sizeof(cli_addr));
    if (bind(sockfd, (sa *)&serv_addr, sizeof(serv_addr)) != 0)
    {
        perror("Unable to bind!");
        exit(0);
    }
    else
        printf("Socket successfully binded!");
    char b1[MAX];

    for (;;)
    {
        bzero(&b1, sizeof(b1));
        if (recvfrom(sockfd, &b1, sizeof(b1), 0, (sa *)&cli_addr, &len) < 0)
        {
            perror("Unable to recieve from Client!");
            exit(0);
        }
        else
        {
            if (strncmp("exit", b1, 4) == 0)
            {
                printf("Client exited, server also exiting");
                break;
            }
            printf("Client texted : %s\n", b1);
        }

        if (sendto(sockfd, &b1, sizeof(b1), 0, (sa *)&cli_addr, len) < 0)
        {
            perror("Unable to send to client!");
            exit(0);
        }
        else
            printf("Echoed to client : %s", b1);
    }
    close(sockfd);
    return 0;
}
