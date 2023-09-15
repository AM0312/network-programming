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
    printf("Starting Echo socket server!\n");

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int sock_opt_len = 1;

    struct sockaddr_in server_addr, client_addr;

    if (sockfd == -1)
    {
        perror("Unable to create socket!");
        exit(0);
    }
    else
        printf("Server socket created!\n");
    int status = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sock_opt_len, sizeof(sock_opt_len));

    if (status < 0)
    {
        perror("Socket option creation failed!");
        exit(0);
    }
    else
        printf("Socket option REUSEADDR successfully added!\n");

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;                // socket internet family
    server_addr.sin_port = htons(PORT);              // socket internet port
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // socket address

    if (bind(sockfd, (sa *)&server_addr, sizeof(server_addr)) != 0)
    {
        perror("Unable to bind socket!");
        exit(0);
    }
    else
        printf("Successfully binded socket on port %d\n", PORT);

    if (listen(sockfd, 5) != 0)
    {
        perror("Server unable to listen!");
        exit(0);
    }
    else
        printf("Server listening on port %d\n", PORT);

    int cli_len = sizeof(client_addr);
    int clifd = accept(sockfd, (sa *)&client_addr, &cli_len);

    if (clifd < 0)
    {
        perror("Socket unable to accept connections!");
        exit(0);
    }
    else
        printf("Connection established with client!\n");
    echo_fn(clifd);
    close(sockfd);
    return 0;
}