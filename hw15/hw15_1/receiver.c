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

void func(int x)
{
    printf("Signal was received!\n");
}

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

    struct sigaction sig = {0};

    sig.sa_handler = func;

    ret = sigaction(SIGUSR1, &sig, NULL);
    if (-1 == ret)
    {
        perror("sigaction() failed");
    }

    while(1)
    {
        sleep(1);
    }

    return 0;
}
