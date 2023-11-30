#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
 
int main()
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9000);
    servaddr.sin_addr.s_addr = inet_addr("172.16.175.255");

    int broadcast = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));

    char string[100];
    bzero(string, 100);
    while (1)
    {
        printf("Enter the message");
        fgets(string, 100, stdin);
        sendto(sockfd, string, 100, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
    }
}
