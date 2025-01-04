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

mqd_t qd_sent = 0;
mqd_t qd_receive = 0;

void func()
{
    int ret = 0;
    
    ret = mq_close(qd_sent);
    if (-1 == ret)
    {
        perror("\nClosing queue error");
    }
 
    ret = mq_unlink(INPUT_MESSAGE_QUEUE);
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

    struct Message message = {0};
    message.id = getpid();
    strcpy(message.name, "Maria");
    message.can_i_join = true;
    snprintf(message.queue_name, sizeof(message.queue_name), TESAT, 228);
    printf("Queue name: %s\n", message.queue_name);

    int prio1 = 0;

    struct mq_attr attr_sent = {0};
    attr_sent.mq_msgsize = sizeof(struct Message);;
    attr_sent.mq_maxmsg = MESSAGENUMBER;

    qd_sent = mq_open(INPUT_MESSAGE_QUEUE, O_CREAT | O_RDWR, 0600, &attr_sent);
    if (-1 == qd_sent)
    {
        perror("\nCreating messages queue failed");
        func();
        return -1;
    }

    struct mq_attr attr_receive = {0};
    attr_receive.mq_msgsize = sizeof(struct Message);;
    attr_receive.mq_maxmsg = MESSAGENUMBER;

    qd_receive = mq_open(message.queue_name, O_CREAT | O_RDWR, 0600, &attr_receive);
    if (-1 == qd_sent)
    {
        perror("\nCreating messages queue failed");
        func();
        return -1;
    }
    
    int count = 0;

    char txt[MESSAGESIZE] = {0};

    printf("Enter message: ");
    scanf("%s", message.text);

    mq_send(qd_sent, (const char *)&message, sizeof(message), prio1);

    while(1)
    {
        mq_receive(qd_receive, (char *)&message, sizeof(message), &prio1);
        printf("%s\n", message.text);
        message.can_i_join = false;

    }

    while(1)
    {
        printf("Enter message: ");
        scanf("%s", message.text);
        printf("%s\n", message.name);

        


        printf("%s: %s\n", message.name, txt);
    }

    return 0;
}