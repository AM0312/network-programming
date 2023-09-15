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
        read(clifd, &msg_buff, sizeof(msg_buff));
        if (strlen(msg_buff) != 0)
        {
            printf("Message from Client : %s\n", msg_buff);
            char rev_buff[MAX];
            bzero(&rev_buff, sizeof(rev_buff));
            int i = 0;
            int n = strlen(msg_buff) - 1;
            while (n >= 0)
                // reversal of message
                rev_buff[i++] = msg_buff[n--];

            write(clifd, &rev_buff, sizeof(rev_buff));
        }
        else
        {
            printf("Client has exited! Server is also exiting now!\n");
            break;
        }
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

    struct sockaddr_in serv_addr, cli_addr;

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (sa *)&serv_addr, sizeof(serv_addr)) != 0)
    {
        perror("Unable to bind socket!\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket successfully binded to port %d\n", PORT);

    if (listen(sockfd, 10) != 0)
    {
        perror("Socket unable to listen on port!\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket is listening on port %d\n", PORT);

    int len_cli = sizeof(cli_addr);
    int clifd = accept(sockfd, (sa *)&cli_addr, &len_cli);
    if (clifd < 0)
    {
        perror("Server is unable to accept connection requests!\n");
        exit(EXIT_FAILURE);
    }
    else
        printf("Socket connection accepted!");
    rev_func(clifd);
    close(sockfd);
    return 0;
}