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

int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("Socket not created!\n");
        exit(EXIT_FAILURE);
    }
    printf("UDP Client Socket created!\n");
    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serv_addr.sin_port = htons(PORT);
    char b1[MAX], b2[MAX];
    int len = sizeof(serv_addr);
    for (;;)
    {
        bzero(&b1, sizeof(b1));
        bzero(&b2, sizeof(b2));
        printf("Enter your message : ");
        fgets(b1, sizeof(b1), stdin);

        if (sendto(sockfd, &b1, sizeof(b1), 0, (sa *)&serv_addr, len) < 0)
        {
            perror("Couldn't send data!");
            exit(0);
        }
        if (strncmp(b1, "exit", 4) == 0)
        {
            perror("Chat ended by client.\n");
            break;
        }
        if (recvfrom(sockfd, &b2, sizeof(b2), 0, (sa *)&serv_addr, &len) < 0)
        {
            perror("Cannot receive message from Server!");
            exit(0);
        }
        printf("Server texted : %s\n", b2);
    }
    close(sockfd);
    return 0;
}