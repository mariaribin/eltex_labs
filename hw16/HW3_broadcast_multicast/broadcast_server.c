#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main()
{
    int server_socket = 0;
    int ret = 0;
    char server_message[10] = {"Hello!"};

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == server_socket)
    {
        perror("Creating socket failed");
        return -1;
    }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(5000);
    address.sin_addr.s_addr = INADDR_BROADCAST;

    socklen_t len = sizeof(address);

    int flag = 1;
    ret = setsockopt(server_socket, 
               SOL_SOCKET, SO_BROADCAST, 
               &flag, sizeof(flag));
    if (-1 == ret)
    {
        perror("setsockopt() failed");
        close(server_socket);
        return -1;
    }

    ret = sendto(server_socket, 
                 &server_message, 
                 sizeof(server_message), 
                 0,(const struct sockaddr *)&address, len);
    if (-1 == ret)
    {
        perror("Sending to client failed");
        close(server_socket);
        return -1;
    }

    close(server_socket);
    return 0;
}
