#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>

int main()
{
    int server_socket = 0;
    int client_socket = 0;
    int ret = 0;
    char server_message[10] = {"Hello!"};
    char message[10] = {0};

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == server_socket)
    {
        perror("Creating socket failed");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    ret = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if (-1 == ret)
    {
        perror("Binding to a socket failed");
        close(server_socket);
        return -1;
    }

    ret = listen(server_socket, 3);
    if (-1 == ret)
    {
        perror("Listening failed");
        close(server_socket);
        return -1;
    }

    while(1)
    {
        client_socket = accept(server_socket, NULL, NULL);
        if (-1 == client_socket)
        {
            perror("Accepting client failed.");
            close(server_socket);
            close(client_socket);
            return -1;
        }

        printf("\nsocket %d\n", client_socket);

        pid_t pid = fork();
        if (0 == pid)
        {
            close(server_socket);

            printf("\nThis is a new process %d with socket %d\n", getpid(), client_socket);

            ret = recv(client_socket, &message, sizeof(message), 0);
            if (-1 == ret)
            {
                perror("Reception from client failed.");
                close(client_socket);
                return -1;
            }

            printf("Client sent: %s\n", message);

            ret = send(client_socket, server_message, sizeof(server_message), 0);
            if (-1 == ret)
            {
                perror("Sending to client failed");
                close(client_socket);
                return -1;
            }

            close(client_socket);
            exit(0);
        }
        else
        {
            close(client_socket);
            while (waitpid(-1, NULL, WNOHANG) > 0);   
        }  
    }

    close(server_socket);
    return 0;
}