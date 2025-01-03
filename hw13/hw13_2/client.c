/*Написать 2 программы, первая сервер, вторая клиент. Сервер создает
очередь сообщений для реализации чата с общей комнатой (если
необходимо, то можно и больше очередей создать) и его задача
уведомлять клиентов о появлении новых участников, о новых
сообщениях. Клиент подключается к очереди, созданной сервером,
сообщает ему свое имя и получает в ответ все сообщения в комнате.
Далее может отправлять сообщения в общий чат. Получение служебных
сообщений от сервера (имена новых клиентов, сообщения от других
пользователей) и отправка сообщений в чат лучше реализовать в разных
потоках. Интерфейс клиента реализуем с помощью библиотеки ncurses.

THIS IS THE CLIENT*/

#include <fcntl.h>  
#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "history.h"
#define MESSAGENUMBER 10
#define SENT "/new_queue"

mqd_t qd_sent = 0;

void func()
{
    int ret = 0;
    
    ret = mq_close(qd_sent);
    if (-1 == ret)
    {
        perror("\nClosing queue error");
    }
 
    ret = mq_unlink(SENT);
    if (-1 == ret)
    {
        perror("\nQueue already removed");
    }

    printf("\ncleaned queue!\n");

    exit(0);
}

int main()
{   
    struct sigaction sig = {0};
    sig.sa_handler = func;
    int ret = sigaction(SIGINT, &sig, NULL);
    if (-1 == ret)
    {
        perror("\nsigaction() failed");
    }
    
    int prio1 = 0;
    //char message[MESSAGESIZE] = {0};
    //char member_name[MESSAGESIZE] = {0};

    struct mq_attr attr_sent;
    attr_sent.mq_msgsize = sizeof(struct Message);;
    attr_sent.mq_maxmsg = MESSAGENUMBER;

    struct Message message = {0};
    message.id = getpid();
    strcpy(message.name, "Maria");

    printf("%s\n", message.name);
    qd_sent = mq_open(SENT, O_CREAT | O_RDWR, 0600, &attr_sent);
    if (-1 == qd_sent)
    {
        perror("\nCreating messages queue failed");
        func();
        return -1;
    }

    int count = 0;

    while(1)
    {
        printf("Enter message: ");
        scanf("%s", message.text);
            printf("%s\n", message.name);

        mq_send(qd_sent, (const char *)&message, sizeof(message), 1);
    }

    return 0;
}