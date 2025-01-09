#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BUFSIZE 32

int main()
{
    int network_socket = 0;
    int ret = 0;
    char response[BUFSIZE] = {0};

    network_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == network_socket)
    {
        perror("Creating socket failed");
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(5000);
    address.sin_addr.s_addr = INADDR_BROADCAST;

    ret = bind(network_socket, 
               (const struct sockaddr*)&address, 
               sizeof(address));
    if (-1 == ret)
    {
        perror("Binding to socket failed.");
        close(network_socket);
        return -1;
    }

    ret = recv(network_socket, &response, sizeof(response), 0);
    if (-1 == ret)
    {
        perror("Reception from server failed.");
        close(network_socket);
        return -1;
    }

    printf("Server responded: %s\n", response);
    close(network_socket);
    return 0;
}
