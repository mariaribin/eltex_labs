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
    pid_t pid_child_1 = 0;
    pid_t pid_child_2 = 0;
    pid_t pid_child_3 = 0;
    pid_t pid_child_4 = 0;
    pid_t pid_child_5 = 0;
    int wstatus_1 = 0;
    int wstatus_2 = 0;
    int wstatus_3 = 0;
    int wstatus_4 = 0;
    int wstatus_5 = 0;

    pid_t ret1 = fork();
    if(-1 == ret1)
    {
        perror("fork failed");
        exit(-1);
    }
    
    if(0 == ret1)
    {
        printf("Child process 1. My pid = %d My ppid = %d\n", 
        getpid(), getppid());

        pid_t ret3 = fork();
        if(-1 == ret3)
        {
            perror("fork failed");
            exit(-1);
        }

        if(0 == ret3)
        {
            printf("Child process 3. My pid = %d My ppid = %d\n", 
            getpid(), getppid());
            exit(0);
        }
        else
        {
            pid_t ret4 = fork();
            if(-1 == ret4)
            {
                perror("fork failed");
                exit(-1);
            }

            if(0 == ret4)
            {
                printf("Child process 4. My pid = %d My ppid = %d\n", 
                getpid(), getppid());
                exit(0);
            }
            else
            {   
                pid_child_3 = waitpid(ret3, &wstatus_3, WUNTRACED);
                if(-1 == pid_child_3)
                {
                    perror("wait failed");
                    exit(-1);
                }

                pid_child_4 = waitpid(ret4, &wstatus_4, WUNTRACED);
                if(-1 == pid_child_3)
                {
                    perror("wait failed");
                    exit(-1);
                }

                int child_ret_3 = WIFEXITED(wstatus_3);
                int child_ret_4 = WIFEXITED(wstatus_4);
        
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

        pid_t ret2 = fork();
        if(-1 == ret2)
        {
            perror("fork failed");
            exit(-1);
        }

        if(0 == ret2)
        {
            printf("Child process 2. My pid = %d My ppid = %d\n", 
            getpid(), getppid());

            pid_t ret5 = fork();
            if(-1 == ret5)
            {
                perror("fork failed");
                exit(-1);
            }

            if(0 == ret5)
            {
                printf("Child process 5. My pid = %d My ppid = %d\n", 
                getpid(), getppid());
                exit(0);
            }

            pid_child_5 = waitpid(ret5, &wstatus_5, WUNTRACED);
            if(-1 == pid_child_5)
            {
                perror("wait failed");
                exit(-1);
            }

            int child_ret_5 = WIFEXITED(wstatus_5);
            printf("Child process with pid %d returned status %d\n", 
            pid_child_5, child_ret_5);

            exit(0);
        }
        else
        {
            pid_child_1 = waitpid(ret1, &wstatus_1, WUNTRACED);
            if(-1 == pid_child_1)
            {
                perror("wait failed");
                exit(-1);
            }

            pid_child_2 = waitpid(ret2, &wstatus_2, WUNTRACED);
            if(-1 == pid_child_2)
            {
                perror("wait failed");
                exit(-1);
            }

            int child_ret_1 = WIFEXITED(wstatus_1);
            int child_ret_2 = WIFEXITED(wstatus_2);
        
            printf("Child process with pid %d returned status %d\n", 
            pid_child_1, child_ret_1);
            printf("Child process with pid %d returned status %d\n", 
            pid_child_2, child_ret_2);   
        }
    }
    return 0;
}