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
    char message[BUFSIZE] = {0};

    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == network_socket)
    {
        perror("Creating socket failed");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    ret = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if (-1 == ret)
    {
        perror("Connection to server failed. Try again.");
        close(network_socket);
        return -1;
    }

    printf("Type message for server: ");
    fgets(message, sizeof(message), stdin);

    ret = send(network_socket, message, sizeof(message), 0);
    if (-1 == ret)
    {
        perror("Sending to server failed");
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