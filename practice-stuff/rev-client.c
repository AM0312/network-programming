#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <sys/types.h>

#define MAX 80
#define PORT 8080
#define sa struct sockaddr

void rev_func(int clifd)
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
    printf("Starting socket server!\n");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Couldn't create a socket!\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket created!\n");
    int optval = 1;
    int status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (status < 0)
    {
        perror("Socket options not set\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket option REUSEADDR successfully applied\n");

    struct sockaddr_in serv_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    serv_addr.sin_port = htons(PORT);

    if (connect(sockfd, (sa *)&serv_addr, sizeof(serv_addr)) != 0)
    {
        perror("Server is not accepting connection requests!\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Client connected! Start texting now!\n");
    rev_func(sockfd);
    close(sockfd);
    return 0;
}