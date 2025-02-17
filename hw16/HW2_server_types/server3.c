/*Сервер с очередью сообщений*/


#include <fcntl.h>  
#include <mqueue.h>
#include <string.h>
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
#define MESSAGESIZE 1
#define MESSAGENUMBER 10
#define QUEUE_NAME "/queue"
#define THREADS 4
#define BUFSIZE 32

struct Data
{ 
    mqd_t *p_qd;
};

static void *func(void *args)
{
    int ret = 0;
    int prio = 0;
    int client_socket = 0;
    char message[BUFSIZE] = {0};
    char socket[MESSAGESIZE] = {0};
    char server_message[10] = {"Hello!"};

    struct Data *t_data = (struct Data *)args;
    if (!t_data)
    {
        return NULL;
    }

    while(1)
    {
        ret = mq_receive(*t_data->p_qd, socket, MESSAGESIZE, &prio);
        if (-1 == ret)
        {
            perror("Message receive failed");
            break;
        }

        printf("\nreceived socket number %d\n", *t_data->p_qd);

        int client_socket = (int)socket[0];

        ret = recv(client_socket, message, sizeof(message), 0);
        if (-1 == ret)
        {
            perror("Reception from client failed.");
            close(client_socket);
            break;
        }

        ret = send(client_socket, server_message, sizeof(server_message), 0);
        if (-1 == ret)
        {
            perror("Sending to client failed");
            close(client_socket);
            break;
        }
    }
}

int main()
{
    int server_socket = 0;
    int ret = 0;
    mqd_t qd = 0;

    struct Data data[THREADS] = {0};

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

    ret = bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
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
    
    struct mq_attr queue_attr;
    queue_attr.mq_msgsize = MESSAGESIZE;
    queue_attr.mq_maxmsg = MESSAGENUMBER;

    ret = mq_unlink(QUEUE_NAME);
    if (-1 == ret)
    {
        perror("Queue already removed");
    }

    qd = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0600, &queue_attr);
    if (-1 == qd)
    {
        perror("Creating queue failed");
        return -1;
    }

    for (int i = 0; i < THREADS; i++)
    {
        data[i].p_qd = &qd;
    }

    pthread_t tid[THREADS] = {0};
    int status[THREADS] = {1, 1, 1, 1};

    for (int i = 0; i < THREADS; i++)
    {
        ret = pthread_create(&tid[i], NULL, func, &data[i]);
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

        char sock_buf[1] = {(char)sock};

        ret = mq_send(qd, sock_buf, MESSAGESIZE, 1);
        if (-1 == ret)
        {
            perror("Sending message failed");
            return -1;
        }
    }
    
    return 0;
}
