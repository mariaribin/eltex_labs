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

void func(int x)
{
    printf("Signal SIGUSR1 was received!\n");
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

    sigset_t blockset;
    sigemptyset(&blockset);
    sigaddset(&blockset,  SIGUSR1);
    sigprocmask(SIG_BLOCK, &blockset, NULL);

    int sig = 0;

    while(1)
    {
        printf("Waiting for signal....\n");
        ret = sigwait(&blockset, &sig);
        if (ret == 0)
        {
            printf("Signal SIGUSR1 was received!\n");
        }
    }

    return 0;
}
