#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#define ADDRESS_SERVER "serversocket"
#define ADDRESS_CLIENT "clientsocket"

int main()
{
    int network_socket = 0;
    int ret = 0;
    char message[10] = {0};

    struct sockaddr_un server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, ADDRESS_SERVER);
    
    struct sockaddr_un client_address;
    memset(&client_address, 0, sizeof(client_address));
    client_address.sun_family = AF_UNIX;
    strcpy(client_address.sun_path, ADDRESS_CLIENT);

    network_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == network_socket)
    {
        perror("Creating socket failed");
        return -1;
    }

    unlink(client_address.sun_path);
    ret = bind(network_socket, (struct sockaddr*) &client_address, sizeof(client_address));
    if (-1 == ret)
    {
        perror("Binding to a socket failed");
        close(network_socket);
        unlink(client_address.sun_path);
        return -1;
    }

    ret = connect(network_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if (-1 == ret)
    {
        perror("Connection to server failed. Try again.");
        close(network_socket);
        unlink(client_address.sun_path);
        return -1;
    }

    printf("Type message for server: ");
    fgets(message, sizeof(message), stdin);
    
    ret = write(network_socket, message, sizeof(message));
    if (-1 == ret) 
    {
        perror("Write into socket failed");
        close(network_socket);
        unlink(client_address.sun_path);
        return -1;
    }

    ret = read(network_socket, message, sizeof(message));
    if (-1 == ret) 
    {
        perror("Read from socket failed");
        close(network_socket);
        unlink(client_address.sun_path);
        return -1;
    }
    
    printf("Server responded: %s\n", message);

    close(network_socket);
    unlink(client_address.sun_path);
    return 0;
}
