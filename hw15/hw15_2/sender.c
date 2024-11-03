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
    pid_t fd = open("pid.txt", O_RDONLY, 0600);
    if (-1 == fd)
    {
        perror("open() file failed");
    }

    int buf = 0;

    int ret = read(fd, &buf, sizeof(buf));
    if (-1 == ret)
    {
        perror("read() from file failed");
    }

    kill(buf, SIGINT);

    close(fd);

    return 0;
}
