/*Написать RAW-сокет. UDP-сервер ждет строку от клиента на RAW-сокете 
и отвечает приветственным сообщением. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define ADDR "192.168.0.114"

int main()
{
    int server_socket = 0;
    int client_socket = 0;
    int ret = 0;
    char server_message[10] = {"Hello!"};
    char message[10] = {0};

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == server_socket)
    {
        perror("Creating socket failed");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);
    inet_pton(AF_INET, ADDR, &server_address.sin_addr.s_addr);

    ret = bind(server_socket, (const struct sockaddr*)&server_address, sizeof(server_address));
    if (-1 == ret)
    {
        perror("Binding to a socket failed");
        close(server_socket);
        return -1;
    }

    struct sockaddr addr;
    socklen_t len = sizeof(addr);

    ret = recvfrom(server_socket, &message, sizeof(message), 
                   0, (struct sockaddr *restrict)&addr, &len);
    if (-1 == ret)
    {
        perror("Reception from client failed.");
        close(server_socket);
        return -1;
    }

    printf("Client sent: %s\n", message);
    fflush(stdout);

    ret = sendto(server_socket, server_message, sizeof(server_message), 
                 0, (struct sockaddr *restrict)&addr, len);
    if (-1 == ret)
    {
        perror("Sending to client failed");
        close(server_socket);
        return -1;
    }

    close(server_socket);
    return 0; 
}
