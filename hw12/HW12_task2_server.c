/*Реализовать 2 программы, первая сервер, вторая клиент. Сервер создает
именованный канал и открывает его только на запись, записывает строку
“Hi!” и завершается. Клиент открывает созданный канал сервером
только на чтение, считывает строку из канала и выводит на экран. Удает
канал.

THIS IS THE SERVER*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUFSIZE 4

int main()
{
    int fd_fifo = 0;
    int ret = 0;
    char buf[BUFSIZE] = "Hi!";

    ret = mkfifo("fifo", 0666);
    if (-1 == ret)
    {
        perror("server mkfifo() failed");
        exit(-1);
    }

    fd_fifo = open("fifo", O_WRONLY);
    if (-1 == fd_fifo)
    {
        perror("server pipe open() failed");
        exit(-1);
    }

    ret = write(fd_fifo, buf, sizeof(buf));
    if (-1 == ret)
    {
        perror("server pipe write() failed");
        exit(-1);
    }

    close(fd_fifo);
    return 0;
}
