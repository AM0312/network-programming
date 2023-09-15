#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <sys/types.h>

#define MAX 80
#define PORT 8000
#define sa struct sockaddr

void echo_fn(int clifd)
{

    int n;
    char msg_buff[MAX];
    bzero(&msg_buff, sizeof(msg_buff));
    for (;;)
    {

        bzero(&msg_buff, sizeof(msg_buff));
        printf("Type exit to exit else write your message and press Enter to send!\n");
        printf("Enter your Message : \n");
        n = 0;
        while ((msg_buff[n++] = getchar()) != '\n')
            ;
        if (strncmp("exit", msg_buff, 4) == 0)
        {
            printf("Client exiting!");
            break;
        }
        write(clifd, &msg_buff, sizeof(msg_buff));
        bzero(&msg_buff, sizeof(msg_buff));
        read(clifd, &msg_buff, sizeof(msg_buff));
        printf("Server Echoed : %s\n", msg_buff);
    }
    return;
}

int main()
{
    printf("Starting Echo socket server!\n");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;

    if (sockfd == -1)
    {
        perror("Unable to create socket!");
        exit(0);
    }
    else
        printf("Client socket created!\n");

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(sockfd, (sa *)&server_addr, sizeof(server_addr)) != 0)
    {
        perror("Client unable to connect with server!");
        exit(0);
    }
    else
        printf("Connection established with server! Start Typing \n");
    echo_fn(sockfd);
    close(sockfd);
    return 0;
}