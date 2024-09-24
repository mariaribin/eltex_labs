/*Реализовать программу, которая порождает процесс. Родительский
процесс и дочерний выводят на экран свой pid, ppid. Затем родительский
ждет завершения дочернего и выводит его статус завершения.*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid_child = 0;
    int wstatus = 0;
     
    pid_t ret = fork();
    if(-1 == ret)
    {
        perror("fork failed");
        exit(-1);
    }

    if(0 == ret)
    {
        printf("Child process.\n"
                "My pid = %d\n"
                "My ppid = %d\n", getpid(), getppid());
        exit(0);
    }
    else
    {
        printf("Parent process.\n"
                "My pid = %d\n"
                "My ppid = %d\n", getpid(), getppid());

        pid_child = wait(&wstatus);
        if(-1 == pid_child)
        {
            perror("wait failed");
            exit(-1);
        }

        int child_ret = WIFEXITED(wstatus);
       
        printf("Child process with pid %d returned %d\n", pid_child, child_ret);
        exit(0); 
    }
    return 0;
}
