/*Реализовать программу, которая переопределяет диспозицию сигнал
SIGUSR1 с помощью вызова sigaction(), после чего уходит в бесконечный
цикл ожидания. В функции диспозиции сигнала на экран выводится
сообщение о получении сигнала. Также необходимо реализовать вторую
программу, которая будет посылать первой программе сигнал SIGUSR1
через вызов kill(). Дополнительно научится посылать сигнал с помощью
утилиты kill.*/

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
