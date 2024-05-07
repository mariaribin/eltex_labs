/*Напишите программу, которая ищет в введенной строке (с клавиатуры)
введенную подстроку (с клавиатуры) и возвращает указатель на начало
подстроки, если подстрока не найдена в указатель записывается NULL.
В качестве срок использовать статические массивы.*/

#include <stdio.h>
#define SIZE 10


int main()
{
    char string[SIZE] = {0};

    for (int i = 0; i < SIZE; i++)
    {
        scanf("%c", &string[i]);
    }

    char substring[4] = {0};

    for (int i = 0; i < 4; i++)
    {
        scanf("%c", &substring[i]);
    }

    char *p_string = &string[0];
    char *p_substring = &substring[0];
 
    for (int i = 0; i < SIZE; i++)
    {
       if (*p_string == *p_substring)
            {
                printf("%c", *p_string);
                p_substring++;  
            }
            else
            {
                break;
            }
            
       *p_substring = &substring[0];
    }
        

    return 0;
}
