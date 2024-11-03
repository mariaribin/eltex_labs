/*Реализовать программу, которая будет блокировать сигнал SIGINT с
помощью вызова sigprocmask() и уходить в бесконечный цикл. Проверить
работу программы с помощью второй программы из задания 1 и утилиты
kill.*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int buf = getpid();

    pid_t fd = open("pid.txt", O_CREAT | O_RDWR, 0600);
    if (-1 == fd)
    {
        perror("open() file failed");
    }

    int ret = write(fd, &buf, sizeof(buf));
    if (-1 == ret)
    {
        perror("write() into file failed");
    }

    close(fd);

    sigset_t blockset;
    sigemptyset(&blockset);
    sigaddset(&blockset, SIGINT);
    sigprocmask(SIG_BLOCK, &blockset, NULL);

    while(1)
    {
        sleep(1);
    }

    return 0;
}
