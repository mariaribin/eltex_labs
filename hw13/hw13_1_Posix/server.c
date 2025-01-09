/*Реализовать 2 программы, первая сервер, вторая клиент. Сервер создает
очередь сообщений и записывает сообщение виде строки “Hi!”, ждет
ответа от клиента и выводит на экран, удаляет очередь. Клиент
подключается к очереди считывает сообщение от сервера, выводит на
экран, отвечает серверу сообщением виде строки “Hello!”. Сделать это
как для POSIX, так и для SYSTEM V стандартов.*/

#include <fcntl.h>  
#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MESSAGESIZE 10
#define MESSAGENUMBER 10
#define QUEUE_NAME "/new_queue"

int main()
{
    mqd_t qd = 0;
    int ret = 0;
    int prio = 0;
    char message[MESSAGESIZE] = "Hi!";

    struct mq_attr queue_attr;
    queue_attr.mq_msgsize = MESSAGESIZE;
    queue_attr.mq_maxmsg = MESSAGENUMBER;
    
    qd = mq_open(QUEUE_NAME, O_CREAT | O_RDWR , 0600, &queue_attr);
    if (-1 == qd)
    {
        perror("Creating queue failed");
        return -1;
    }

    ret = mq_send(qd, message, MESSAGESIZE, 1);
    if (-1 == ret)
    {
        perror("Sending message failed");
        return -1;
    }

    sleep(5);

    ret = mq_receive(qd, message, MESSAGESIZE, &prio);
    if (-1 == ret)
    {
        perror("Message receive failed");
        return -1;
    }

    printf("%s\n", message);

    ret = mq_close(qd);
    if (-1 == ret)
    {
        perror("Closing queue error");
    }
 
    ret = mq_unlink(QUEUE_NAME);
    if (-1 == ret)
    {
        perror("Queue already removed");
    }

    return 0;
}
