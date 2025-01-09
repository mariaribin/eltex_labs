/*Реализовать аналог командного интерпретатора bash. При запуске
программы пользователю предлагается ввести имя программы и опции
запуска программы. Программа порождает процесс и в нем выполняет
введенную программу с заданными опциями, ждет завершения
дочернего процесса. Снова возвращается к вводу следующей
программы. Выход из интерпретатора по команде exit.

ПЕРЕПИСАНО на каналах с поддержкой конструкций типа ls | grep*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
    char command1[255] = {0};
    char command2[255] = {0};

    char ex[5] = "exit";
    int count = 0;
    int i = 0;
    int ret = 0;
    int flag = 0;
    
    printf("Enter name of program and parameters: ");

    while(1)
    {   
        char sym = getc(stdin);
        if (sym == '|')
        {
            flag = 1;
            i = 0;
            continue;
        }

        if (sym == '\n')
        {
            break;
        }

        if (flag == 1)
        {
            command2[i] = sym;
            i++;
        }
        else
        {
            command1[i] = sym;
            i++;
        }
    }

    ret = strcmp(command1, ex);
    if(0 == ret)
    {
        return 0;
    }
     
    printf("%s\n", command1);
    printf("%s\n", command2);

    if (flag == 1)
    {
        FILE *fpout = popen(command1, "r");

        char buf[10000] = {0};

        i = 0;
        while (1)
        {
            char ch = fgetc(fpout);

            if (ch == EOF)
                break;
            buf[i] = ch;
            i++;
        }

        pclose(fpout);

        FILE *fpin = popen(command2, "w");

        fputs(buf, fpin);

        pclose(fpin);
    }
    else
    {
        FILE *fpout = popen(command1, "w");

        pclose(fpout);
    }

    return 0;
}
