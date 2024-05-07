/*Вывести двоичное представление целого положительного числа, 
используя битовые операции (число вводится с клавиатуры).

Вывести двоичное представление целого отрицательного числа,
используя битовые операции (число вводится с клавиатуры).*/

#include <stdio.h>
#define SIZE 32

int binary_number(int a) 
{
    const unsigned int mask = 1;
    int array[SIZE] = {0};

    for (int i = SIZE - 1; i >= 0 ; i--)
    {
        array[i] = mask & a;
        a = a >> 1;
    }

    int i = 0;

    /*to make sure the first 0's aren't printed,
    we skip them with a FOR cycle*/
    for (i; i < SIZE; i++)
    {
        if (0 == array[i])
        {
            continue;
        }
        break;
    }

    /*once the program reaches the first meaningful number, 
    the program starts printing it and the rest of numbers in the console*/
    for (i; i < SIZE; i++)
    {
        printf("%d", array[i]); 
    }

    return 0;
}

int main()
{
    int a = 0;

    printf("Insert a decimal number: ");
    scanf("%d", &a);
    printf("\nGiven number in binary: ");
    binary_number(a);
    printf("\n");

    return 0;
}
