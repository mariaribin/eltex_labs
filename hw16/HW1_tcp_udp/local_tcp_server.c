#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#define ADDRESS_SERVER "serversocket"

int main()
{
    int server_socket = 0;
    int client_socket = 0;
    int ret = 0;
    char message[10] = {0};
    char server_message[10] = {"Hello!"};

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (-1 == server_socket)
    {
        perror("Creating socket failed");
        return -1;
    }

    struct sockaddr_un server_address;

    memset(&server_address, 0, sizeof(server_address));
    server_address.sun_family = AF_UNIX;
    strcpy (server_address.sun_path, ADDRESS_SERVER);

    unlink(server_address.sun_path);
    ret = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    if (-1 == ret)
    {
        perror("Binding to a socket failed");
        close(server_socket);
        unlink(server_address.sun_path);
        return -1;
    }

    ret = listen(server_socket, 3);
    if (-1 == ret)
    {
        perror("Listening failed");
        close(server_socket);
        unlink(server_address.sun_path);
        return -1;
    }

    client_socket = accept(server_socket, NULL, NULL);
    if (-1 == client_socket)
    {
        perror("Accepting client failed.");
        close(server_socket);
        close(client_socket);
        unlink(server_address.sun_path);
        return -1;
    }

    ret = read(client_socket, message, sizeof(message));
    if (-1 == ret) 
    {
        perror("Read from socket failed");
        close(server_socket);
        close(client_socket);
        unlink(server_address.sun_path);
        return -1;
    }

    printf("Client sent: %s\n", message);
    
    ret = write(client_socket, server_message, sizeof(server_message));
    if (-1 == ret) 
    {
        perror("Write into socket failed");
        close(server_socket);
        close(client_socket);
        unlink(server_address.sun_path);
        return -1;
    }

    close(server_socket);
    close(client_socket);
    unlink(server_address.sun_path);
    return 0;
}
