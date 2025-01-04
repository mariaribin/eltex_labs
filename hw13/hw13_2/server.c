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
#define MESSAGE_PRIO_1 1
#define MESSAGE_PRIO_2 2
#define MESSAGE_PRIO_3 3

mqd_t qd_received = 0;
mqd_t qd_clients[5] = {0};

void func()
{
    int ret = 0;
 
    ret = mq_unlink(INPUT_MESSAGE_QUEUE);
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

    struct Message message[32] = {0};
    strcpy(message[0].text, "Hi");
    strcpy(message[1].text, "Hi1");
    strcpy(message[2].text, "Hi2");
    int prio1 = 0;

    struct mq_attr attr_received = {0};
    attr_received.mq_msgsize = sizeof(struct Message);
    attr_received.mq_maxmsg = MESSAGENUMBER;

    qd_received = mq_open(INPUT_MESSAGE_QUEUE, O_CREAT | O_RDWR, 0600, &attr_received);
    if (-1 == qd_received)
    {
        perror("\nCreating members queue failed");
        return -1;
    }

    int storage_ind = 0;
    int client_ind = 0;
    int history_ind = 0;


    while(1)
    {    
        struct Message tmp = {0}; 
         
        printf("123\n");
        ret = mq_receive(qd_received, (char *)&tmp, sizeof(struct Message), &prio1);
        if (-1 == ret)
        {
            perror("Receive failed");
            break;
        }

        if (tmp.can_i_join == true)
        { 
            printf("%s: %s\n", tmp.name, tmp.text);  
            printf("Queue name = %s\n", tmp.queue_name);

            struct mq_attr attr123_received = {0};
            attr123_received.mq_msgsize = sizeof(struct Message);
            attr123_received.mq_maxmsg = MESSAGENUMBER;

            qd_clients[client_ind] = mq_open(tmp.queue_name, O_CREAT | O_RDWR, 0600, &attr123_received);
            if (-1 == qd_clients[client_ind])
            {
                perror("Open queue failed");
                break;
            }


            while(1)
            {
                ret = mq_send(qd_clients[client_ind], (const char *)&message[history_ind], sizeof(struct Message), prio1);
                if (-1 == ret)
                {
                    perror("Send failed");
                    break;
                }

                history_ind++;

                if (message[history_ind].text[0] == 0)
                {
                    ret = mq_send(qd_clients[client_ind], (const char *)&message[history_ind], sizeof(struct Message), prio1);
                    if (-1 == ret)
                    {
                        perror("Send failed");
                        break;
                    } 
                    break;
                }
            }
            client_ind++;
            history_ind = 0;
        }
        else
        {
            printf("%s: %s\n", tmp.name, tmp.text); 
            memcpy(&message[storage_ind], &tmp, sizeof(struct Message));
            
            for (int i = 0; i < client_ind; i++)
            {
                ret = mq_send(qd_clients[i], (const char *)&message[storage_ind], sizeof(struct Message), prio1);
                if (-1 == ret)
                {
                    perror("Send failed");
                    break;
                }
            }

            storage_ind++;
        } 
    }

    func(); 
    return 0;
}