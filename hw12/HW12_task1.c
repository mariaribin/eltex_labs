/*Реализовать программу, которая создает неименованный канал и
порождает процесс. Родительский процесс закрывает дескриптор для
чтения из канала и записывает в канал строку “Hi!”, затем ожидает
завершения дочернего процесса. Дочерний процесс закрывает
дескриптор для записи в канал, считывает строку из канала и выводит на
экран.*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFSIZE 500

int main()
{
    pid_t pid = 0;
    int fd[2] = {0};
    char buf[BUFSIZE] = {0};

    if(-1 == pipe(fd))
    {
        perror("pipe() failed");
        exit(-1);
    }

    pid = fork();
    if (-1 == pid)
    {
        perror("fork() failed");
        exit(-1);
    }
    else if (pid > 0)
    {
        int wstatus = 0;
        int pid_child = 0;
        int ret = 0;

        ret = close(fd[0]);
        if (-1 == ret)
        {
            perror("close() pipe for read failed");
            exit(-1);
        }

        snprintf(buf, sizeof(buf), "Hi!");

        ret = write(fd[1], buf, sizeof(buf));
        if (-1 == ret)
        {
            perror("PARENT write() into pipe failed");
            exit(-1);
        }

        pid_child = wait(&wstatus);
        if (-1 == pid_child)
        {
            perror("wait() failed");
            exit(-1);
        }

        ret = WIFEXITED(wstatus);
        printf("\nChild process with pid %d returned status %d\n",
        pid_child, ret);

        exit(0);
    }
    else if (0 == pid)
    {
        int ret = 0;

        ret = close(fd[1]);
        if (-1 == ret)
        {
            perror("close() pipe for write failed");
            exit(-1);
        }

        ret = read(fd[0], buf, sizeof(buf));
        if (-1 == ret)
        {
            perror("read() from pipe failed");
            exit(-1);
        }

        printf("%s", buf);

        exit(0);
    }
    return 0;
}
