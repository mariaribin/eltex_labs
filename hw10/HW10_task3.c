/*Реализовать аналог командного интерпретатора bash. При запуске
программы пользователю предлагается ввести имя программы и опции
запуска программы. Программа порождает процесс и в нем выполняет
введенную программу с заданными опциями, ждет завершения
дочернего процесса. Снова возвращается к вводу следующей
программы. Выход из интерпретатора по команде exit.*/

#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <string.h>
#define COMMAND_SIZE 20

int main()
{  
    char *args[6] = {0};
    char name[255] = {0};
    char ex[5] = "exit";
    int count = 0;
    int i = 0;
    int ret = 0;

    while(1)
    {   
        args[0] = name;

        printf("Enter name of program and parameters: ");

        while(1)
        {   
            char sym = getc(stdin);

            if ((' ' == sym || '\n' == sym) && 0 == name[0])
            {
                i = 0;
                continue;
            }

            if (' ' == sym && 0 != name[0])
            {
                count++;         
                i = 0;
                continue;
            }

            if ('\n' == sym && count <= 5)
            {
                count++;
                break;
            }

            if ('\n' == sym && count > 5)
            {
                perror("too many arguments");

                for (i = 1; i < 5; i++)
                {
                    free(args[i]);
                    args[i] = NULL;
                }

                memset(name, 0, 255);
                count = 0;
                i = 0;

                printf("Enter name of program and parameters: ");
                continue;
            }
            
            if (!args[count] && count <= 5)
            {
                args[count] = calloc(COMMAND_SIZE, sizeof(char));
            }

            if (0 == count)
            {
                name[i] = sym;
            }
            else if(count <= 5)
            {
                args[count][i] = sym;
            }

            i++;
        }

        ret = strcmp(name, ex);
        if(0 == ret)
        {
            return 0;
        }

        pid_t ret_fork = fork();
        if(-1 == ret_fork)
        {
            perror("fork() failed");
            exit(-1);
        }
        else if(0 == ret_fork)
        {
            ret = execvp(name, args);
            if(-1 == ret)
            {
                perror("execvp() failed");
                exit(-1);
            }

            exit(0);
        }
        else
        {
            int wstatus = 0;
            int pid_child = waitpid(ret_fork, &wstatus, WUNTRACED);
            if(-1 == pid_child)
            {
                perror("wait() failed");
                exit(-1);
            }

            int ret_child = WIFEXITED(wstatus);
            printf("\nChild process with pid %d returned status %d\n",
            pid_child, ret_child);

            //cleaning
            for (i = 1; i <= count; i++)
            {
                free(args[i]);
                args[i] = NULL;
            }
            args[0] = NULL;
            memset(name, 0, 255);
            i = 0;
            count = 0;
        }  
    }
    return 0;
}
