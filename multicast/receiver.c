#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    // Step 1: Create a socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    // Step 2: Set the SO_REUSEADDR option
    int reuse = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // Step 3: Use bind() to specify the local port number
    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY; // Receive datagrams addressed to the multicast group
    local_addr.sin_port = htons(5000);       // Multicast port
    bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr));

    // Step 4: Use the IP_ADD_MEMBERSHIP socket option to join the multicast group
    struct ip_mreq multicast_request;
    multicast_request.imr_multiaddr.s_addr = inet_addr("224.0.0.1"); // Multicast group address
    multicast_request.imr_interface.s_addr = inet_addr("your_local_ip_address");
    setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicast_request, sizeof(multicast_request));

    // Step 5: Receive the datagram
    char buffer[1024];
    while (1)
    {
        int recv_len = recv(sock, buffer, sizeof(buffer), 0);
        buffer[recv_len] = '\0';
        printf("Received message: %s\n", buffer);
    }

    return 0;
}
