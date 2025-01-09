/*Реализовать 2 программы, первая сервер, вторая клиент. Сервер создает
очередь сообщений и записывает сообщение виде строки “Hi!”, ждет
ответа от клиента и выводит на экран, удаляет очередь. Клиент
подключается к очереди считывает сообщение от сервера, выводит на
экран, отвечает серверу сообщением виде строки “Hello!”. Сделать это
как для POSIX, так и для SYSTEM V стандартов.*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAXLEN 10

int main()
{
    key_t key = 0;
    int msqid = 0;
    int len = 0;
    const char pathname[] = "file.c";

    struct message
    {
        long mtype;
        char mtext[MAXLEN];
    } mymessage;

    key = ftok(pathname, 1);
    if (-1 == key)
    {
        perror("ftok() failed");
        exit (-1);
    }

    msqid = msgget(key, 0666 | IPC_CREAT);
    if (-1 == msqid)
    {
        perror("msgget() failed");
        exit (-1);
    }
   
    mymessage.mtype = 1;
    strcpy(mymessage.mtext, "Hi!");
    len = strlen(mymessage.mtext) + 1;
    
    int ret = msgsnd(msqid, (struct message *)&mymessage, len, 0);
    if (-1 == ret)
    {
        perror("msgsnd() failed");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    } 

    sleep(5);

    len = msgrcv(msqid, (struct message *)&mymessage, MAXLEN, 0, 0);
    if (-1 == len)
    {
        perror("msgrcv() failed");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit (-1);
    }

    printf("%s\n", mymessage.mtext);

    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);

    return 0;
}
