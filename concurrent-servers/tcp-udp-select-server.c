// creating an echo TCP server with MAX clients, and an iterative UDP server that returns length of my message.

#define MAX 10
#define PORT 8888
#define sa struct sockaddr

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#define TRUE 1

int max(int x, int y)
{
    return x > y ? x : y;
}

int main()
{
    int tfd, ufd, tcli[MAX], max_cli = MAX, sd, valread, activity;
    int max_sd;
    int new_socket;
    char buff[1025];

    char *tmsg = "ECHO Daemon \r\n";
    char *umsg = "Size Daemon \r\n";

    struct sockaddr_in t_addr;

    fd_set readfds;

    t_addr.sin_family = AF_INET;
    t_addr.sin_port = htons(PORT);
    t_addr.sin_addr.s_addr = INADDR_ANY;

    for (int i = 0; i < max_cli; i++)
        tcli[i] = 0;

    tfd = socket(AF_INET, SOCK_STREAM, 0);
    ufd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(tfd, (sa *)&t_addr, sizeof(t_addr));
    bind(ufd, (sa *)&t_addr, sizeof(t_addr));

    listen(tfd, 3);
    printf("TCP socket listening for connect requests!\n");

    int len = sizeof(t_addr);
    while (TRUE)
    {
        FD_ZERO(&readfds);
        FD_SET(tfd, &readfds);
        FD_SET(ufd, &readfds);
        max_sd = max(ufd, tfd);

        for (int i = 0; i < max_cli; i++)
        {
            sd = tcli[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (max_sd < sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error");
        }
        if (FD_ISSET(tfd, &readfds))
        {
            if ((new_socket = accept(tfd, (sa *)&t_addr, (socklen_t *)&len)) < 0)
            {
                perror("Can't accept!");
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("TCP Client with fd : %d, IP %s, and port %d connected\n", new_socket, inet_ntoa(t_addr.sin_addr), ntohs(t_addr.sin_port));
                for (int i = 0; i < max_cli; i++)
                {
                    if (tcli[i] == 0)
                    {
                        tcli[i] = new_socket;
                        break;
                    }
                }
            }
        }
        for (int i = 0; i < max_cli; i++)
        {
            sd = tcli[i];
            if (FD_ISSET(sd, &readfds))
            {
                if ((valread = read(sd, &buff, 1025)) == 0)
                {
                    getpeername(sd, (sa *)&t_addr, (socklen_t *)&len);
                    close(sd);
                    tcli[i] = 0;
                    printf("Client disconnected with IP %s and port %d !\n", inet_ntoa(t_addr.sin_addr), ntohs(t_addr.sin_port));
                }
                else
                {
                    buff[valread] = '\0';
                    send(sd, &buff, strlen(buff), 0);
                }
            }
        }

        if (FD_ISSET(ufd, &readfds))
        {
            bzero(&buff, sizeof(buff));
            valread = recvfrom(ufd, buff, sizeof(buff), 0, (sa *)&t_addr, (socklen_t *)&len);
            printf("Message from UDP client : ");
            puts(buff);
            bzero(&buff, sizeof(buff));
            sprintf(buff, "%d\n", valread - 1);
            // credits to ansh for sprintf
            buff[valread] = '\0';
            sendto(ufd, &buff, sizeof(buff), 0, (sa *)&t_addr, len);
        }
    }

    return 0;
}