/*Написать программу, в которой реализуется event-loop с помощью
вызова sigwait(). При запуске программа блокирует сигнал SIGUSR1 и
переходит в бесконечный цикл ожидания сигнала, при получении сигнала
на экран выводится сообщение о получении сигнала и осуществляется
переход на следующую итерацию цикла. Проверить программу с помощью
второй программы из задания 1 и утилиты kill.*/

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

    kill(buf, SIGUSR1);

    close(fd);

    return 0;
}
