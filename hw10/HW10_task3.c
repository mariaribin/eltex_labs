/*Реализовать аналог командного интерпретатора bash. При запуске
программы пользователю предлагается ввести имя программы и опции
запуска программы. Программа порождает процесс и в нем выполняет
введенную программу с заданными опциями, ждет завершения
дочернего процесса. Снова возвращается к вводу следующей
программы. Выход из интерпретатора по команде exit.*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>

int main()
{ 
    pid_t pid_child = 0;
    int wstatus = 0;
    int ret_child = 0;
    int ret_strcmp = 0;
    int ret_sprintf = 0;
    char buf[260] = {0};
    char program_name[255] = {0};
    char arg1[10] = {0};
    char ex[5] = "exit";
    bool run = true;

    while(run)
    {
        printf("Enter name of program and flag: ");
        scanf("%255s %10s", program_name, arg1);

        ret_strcmp = strcmp(program_name, ex);
        if(0 == ret_strcmp)
        {
            return 0;
        }
        
        pid_t ret_fork = fork();
        if(-1 == ret_fork)
        {
            perror("fork() failed");
        }

        if(0 == ret_fork)
        {
            pid_t ret_exec = execlp(program_name, program_name, arg1, NULL);
            if(-1 == ret_exec)
            {
                perror("execl() failed");
            }
            exit(0);
        }
        else
        {
            pid_child = waitpid(ret_fork, &wstatus, WUNTRACED);
            if(-1 == pid_child)
            {
                perror("wait failed");
                exit(-1);
            }

            ret_child = WIFEXITED(wstatus);
            printf("\nChild process with pid %d returned status %d\n",
            pid_child, ret_child);
        }
    }
    return 0;
}
