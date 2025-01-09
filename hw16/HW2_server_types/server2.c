/*Сервер со схемой с заранее выделенным пулом обслуж. серверов*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <linux/unistd.h>
#define THREADS 4
#define BUFSIZE 32

struct Data
{ 
    int *p_client_socket;
};

static void *func(void *args) /*Функция получения сообщения от каждого клиента и отправка ему приветствия*/
{
    int ret = 0;
    char message[BUFSIZE] = {0};
    char server_message[10] = {"Hello!"};

    struct Data *data = (struct Data *)args;
    if (!data)
    {
        return NULL;
    }

    while (1)
    {
        while(*data->p_client_socket == 0)
        {
            sleep(1);
        }

        while(*data->p_client_socket > 0)
        {
            ret = recv(*data->p_client_socket, message, sizeof(message), 0);
            if (-1 == ret)
            {
                perror("Reception from client failed.");
                *data->p_client_socket = 0;
                break;
            }

            if (0 == ret)
            {
                *data->p_client_socket = 0;
                break;
            }

            if (ret)
            {
                printf("Client sent: %s\n", message);
                memset(message, 0, sizeof(message));

                ret = send(*data->p_client_socket, server_message, sizeof(server_message), 0);
                if (-1 == ret)
                {
                    printf("\nSending to client failed\n");
                    *data->p_client_socket = 0;
                    break;
                }
            }    
        }  
    }
}

int main()
{
    int server_socket = 0;
    int client_socket[THREADS] = {0};
    int ret = 0;

    struct Data g_data[THREADS] = {0};
    
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

    pthread_t tid[THREADS] = {0};
    int status[THREADS] = {1, 1, 1};

    for (int i = 0; i < THREADS; i++)
    {
        g_data[i].p_client_socket = &client_socket[i];
    }

    for (int i = 0; i < THREADS; i++)
    {
        ret = pthread_create(&tid[i], NULL, func, &g_data[i]);
        if (-1 == ret)
        {
            perror("pthread_create() failed");
            return -1;
        }
    }

    int sock = 0;

    while(1)
    {
        sock = accept(server_socket, NULL, 0);
        if (-1 == sock)
        {
            perror("Accepting client failed.");
        }

        for (int i = 0; i < THREADS; i++)
        {
            if(client_socket[i] == 0)
            {
                client_socket[i] = sock;
                break;
            }
        }
    }

    close(server_socket);
    return 0;
}
