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

THIS IS THE SERVER*/

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
#define RECEIVED "/new_queue"
#define MESSAGE_PRIO_1 1
#define MESSAGE_PRIO_2 2
#define MESSAGE_PRIO_3 3

mqd_t qd_received = 0;

void func()
{
    int ret = 0;
 
    ret = mq_unlink(RECEIVED);
    if (-1 == ret)
    {
        perror("\nQueue already removed");
    }

    ret = mq_close(qd_received);
    if (-1 == ret)
    {
        perror("\nQueue already closed");
    }

    printf("\ncleaned queue!\n");
    exit(0);
}

int main()
{   
    int i = 0;
    
    struct sigaction sig = {0};
    sig.sa_handler = func;
    int ret = sigaction(SIGINT, &sig, NULL);
    if (-1 == ret)
    {
        perror("\nsigaction() failed");
    }

    struct Message message[1024] = {0};
  
    int prio1 = 0;
    char history[5][MESSAGESIZE] = {"One", "Two", "Three", "Four", "Five"};
    //char message[MESSAGESIZE] = {0};
    
    struct mq_attr attr_received;
    attr_received.mq_msgsize = sizeof(struct Message);
    attr_received.mq_maxmsg = MESSAGENUMBER;

    qd_received = mq_open(RECEIVED, O_CREAT | O_RDWR, 0600, &attr_received);
    if (-1 == qd_received)
    {
        perror("\nCreating members queue failed");
        return -1;
    }

    while(1)
    {
        struct Message tmp = {0};
        ret = mq_receive(qd_received, (char *)&tmp, sizeof(tmp), &prio1);
        
        printf("%s: %s\n", tmp.name, tmp.text);
        i++;
    
    }
    
    

    return 0;
}