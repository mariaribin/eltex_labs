/*Напишите программу, которая ищет в введенной строке (с клавиатуры)
введенную подстроку (с клавиатуры) и возвращает указатель на начало
подстроки, если подстрока не найдена в указатель записывается NULL.
В качестве строк использовать статические массивы.*/

#include <stdio.h>
#include <string.h>
#define SIZE1 200
#define SIZE2 200

int main()
{
    char string[SIZE1] = {0};
    char substring[SIZE2] = {0};
    char *p_string = NULL;
    char *p_substring = NULL;

    printf("Type any string: ");
    fgets(string, SIZE1, stdin);  

    printf("\nType any substring to find: ");
    fgets(substring, SIZE2, stdin);

    int const len_str = strlen(string);
    int const len_substr = strlen(substring);
    int counter = 0;

    int j = 0;

    for (int i = 0; i < len_str; i++)
    {
        if (counter == len_substr - 1)
        {
            p_substring = string + i - (len_substr - 1);
            break;
        }

        if (string[i] == substring[j])
        {
            counter++;
            j++;
        }
        else
        {
            j = 0;
            counter = 0;
        }
    }

    if (p_substring != NULL)
    {
        printf("\nThe string has been found. It begins here...| %s\n", p_substring);
    }
    else
    {
        printf("\nThe substring has not been found.\n");
    }

    return 0;
}
