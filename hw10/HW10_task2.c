/*Реализовать программу, которая порождает процесс1 и процесс2, ждет
завершения дочерних процессов. Процесс1 в свою очередь порождает
процесс3 и процесс4 и ждет их завершения. Процесс2 порождает
процесс5 и ждет его завершения. Все процессы выводят на экран свой
pid, ppid.*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int wstatus_1 = 0;
    int wstatus_2 = 0;
    int wstatus_3 = 0;
    int wstatus_4 = 0;
    int wstatus_5 = 0;

    pid_t fork_ret1 = fork();
    if(-1 == fork_ret1)
    {
        perror("fork() failed");
        exit(-1);
    }
    else if(0 == fork_ret1)
    {
        printf("Child process 1. My pid = %d My ppid = %d\n",
        getpid(), getppid());

        pid_t fork_ret3 = fork();
        if(-1 == fork_ret3)
        {
            perror("fork() failed");
            exit(-1);
        }
        else if(0 == fork_ret3)
        {
            printf("Child process 3 (from parent process 1). My pid = %d My ppid = %d\n",
            getpid(), getppid());
            exit(0);
        }
        else
        {
            pid_t fork_ret4 = fork();
            if(-1 == fork_ret4)
            {
                perror("fork() failed");
                exit(-1);
            }
            else if(0 == fork_ret4)
            {
                printf("Child process 4 (from parent process 1). My pid = %d My ppid = %d\n",
                getpid(), getppid());
                exit(0);
            }
            else
            {   
                pid_t pid_child_3 = 0;
                pid_t pid_child_4 = 0;
                int child_ret_3 = 0;
                int child_ret_4 = 0;
                
                pid_child_3 = waitpid(fork_ret3, &wstatus_3, WUNTRACED);
                if(-1 == pid_child_3)
                {
                    perror("wait() failed");
                    exit(-1);
                }

                pid_child_4 = waitpid(fork_ret4, &wstatus_4, WUNTRACED);
                if(-1 == pid_child_3)
                {
                    perror("wait() failed");
                    exit(-1);
                }

                child_ret_3 = WIFEXITED(wstatus_3);
                child_ret_4 = WIFEXITED(wstatus_4);

                printf("Child process with pid %d returned status %d\n",
                pid_child_3, child_ret_3);
                printf("Child process with pid %d returned status %d\n",
                pid_child_4, child_ret_4);
            }
        }
        exit(0);
    }
    else
    {
        printf("Parent process. My pid = %d My ppid = %d\n", 
        getpid(), getppid());

        pid_t fork_ret2 = fork();
        if(-1 == fork_ret2)
        {
            perror("fork() failed");
            exit(-1);
        }
        else if(0 == fork_ret2)
        {
            pid_t fork_ret5 = 0;
            pid_t pid_child_5 = 0;
            int child_ret_5 = 0;

            printf("Child process 2. My pid = %d My ppid = %d\n",
            getpid(), getppid());

            fork_ret5 = fork();
            if(-1 == fork_ret5)
            {
                perror("fork() failed");
                exit(-1);
            }
            else if(0 == fork_ret5)
            {
                printf("Child process 5 (from parent process 2). My pid = %d My ppid = %d\n",
                getpid(), getppid());
                exit(0);
            }
            else
            {
                pid_child_5 = waitpid(fork_ret5, &wstatus_5, WUNTRACED);
                if(-1 == pid_child_5)
                {
                    perror("wait failed");
                    exit(-1);
                }

                child_ret_5 = WIFEXITED(wstatus_5);
                printf("Child process with pid %d returned status %d\n",
                pid_child_5, child_ret_5);

                exit(0);
            }
        }
        else
        {
            pid_t pid_child_1 = 0;
            pid_t pid_child_2 = 0;
            int child_ret_1 = 0;
            int child_ret_2 = 0;
            
            pid_child_1 = waitpid(fork_ret1, &wstatus_1, WUNTRACED);
            if(-1 == pid_child_1)
            {
                perror("wait() failed");
                exit(-1);
            }

            pid_child_2 = waitpid(fork_ret2, &wstatus_2, WUNTRACED);
            if(-1 == pid_child_2)
            {
                perror("wait() failed");
                exit(-1);
            }

            child_ret_1 = WIFEXITED(wstatus_1);
            child_ret_2 = WIFEXITED(wstatus_2);

            printf("Child process with pid %d returned status %d\n",
            pid_child_1, child_ret_1);
            printf("Child process with pid %d returned status %d\n",
            pid_child_2, child_ret_2);   
        }
    }
    return 0;
}
