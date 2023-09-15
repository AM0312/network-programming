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
        printf("Client socket created!\n");

    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serv_addr.sin_port = htons(PORT);

    int len = sizeof(serv_addr);
    char b1[MAX];
    for (;;)
    {
        bzero(&b1, sizeof(b1));
        printf("Enter your message : ");
        fgets(b1, sizeof(b1), stdin);
        if (sendto(sockfd, &b1, sizeof(b1), 0, (sa *)&serv_addr, len) < 0)
        {
            perror("Unable to send a message to the server!");
            exit(0);
        }
        else
        {
            if (strncmp("exit", b1, 4) == 0)
            {
                printf("Client exiting!");
                break;
            }
        }
        bzero(&b1, sizeof(b1));
        if (recvfrom(sockfd, &b1, sizeof(b1), 0, (sa *)&serv_addr, &len) < 0)
        {
            perror("Unable to recieve from server!");
            exit(0);
        }
        else
            printf("Server echoed : %s\n", b1);
    }
    close(sockfd);
    return 0;
}
