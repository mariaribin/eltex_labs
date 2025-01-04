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
#include <pthread.h>
#include "history.h"

mqd_t qd_sent = 0;
mqd_t qd_receive = 0;

struct Arg
{
    struct Message *p_message;

};

void clean()
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

void* send(void *arg)
{
    while (1)
    {
        struct Arg *args = (struct Arg *)arg;
        if (!args)
        {
            return NULL;
        }

        printf("Enter message: ");
        scanf("%s", args->p_message->text);
        mq_send(qd_sent, (const char *)args->p_message, sizeof(struct Message), PRIO);
    }
}

int main()
{   
    struct sigaction sig = {0};
    sig.sa_handler = clean;
    int ret = sigaction(SIGINT, &sig, NULL);
    if (-1 == ret)
    {
        perror("\nsigaction() failed");
    }

    struct Message message = {0};
    message.id = getpid();
    char buf[10] = {0};

    printf("Enter name: ");
    scanf("%s", buf);
    strcpy(message.name, buf);

    message.can_i_join = true;
    snprintf(message.queue_name, sizeof(message.queue_name), MYQUEUE, getpid());

    struct Message tmp = {0};

    int prio1 = 0;

    struct mq_attr attr_sent = {0};
    attr_sent.mq_msgsize = sizeof(struct Message);;
    attr_sent.mq_maxmsg = MESSAGENUMBER;

    qd_sent = mq_open(INPUT_MESSAGE_QUEUE, O_CREAT | O_RDWR, 0600, &attr_sent);
    if (-1 == qd_sent)
    {
        perror("\nCreating messages queue failed");
        clean();
        return -1;
    }

    struct mq_attr attr_receive = {0};
    attr_receive.mq_msgsize = sizeof(struct Message);;
    attr_receive.mq_maxmsg = MESSAGENUMBER;

    qd_receive = mq_open(message.queue_name, O_CREAT | O_RDWR, 0600, &attr_receive);
    if (-1 == qd_sent)
    {
        perror("\nCreating messages queue failed");
        clean();
        return -1;
    }
  
    mq_send(qd_sent, (const char *)&message, sizeof(struct Message), PRIO);

    for (int i = 0; i < 32; i++)
    {
        mq_receive(qd_receive, (char *)&tmp, sizeof(struct Message), &prio1);
        printf("%s: %s\n", tmp.name, tmp.text);
        if (true == message.can_i_join)
        {
            message.can_i_join = false;
        }
        if (0 == strlen(tmp.text))
        {
            break;
        }
    }

    struct Arg arg = {0};
    arg.p_message = &message;

    pthread_t thread = 0;
    pthread_create(&thread, NULL, send, &arg);
    pthread_detach(thread);

    while(1)
    {
        mq_receive(qd_receive, (char *)&tmp, sizeof(struct Message), &prio1);

        if (2 == prio1)
        {
            printf("%s\n", tmp.text);
        }
        else
        {
            printf("%s: %s\n", tmp.name, tmp.text);
        }
        //if (0 != strcmp(tmp.queue_name, message.queue_name))
       // {
            //printf("%s: %s\n", tmp.name, tmp.text);
       // } 
        
    }

    clean();
    return 0;
}