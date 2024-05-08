/*Найти количество единиц в двоичном представлении целого
положительного числа (число вводится с клавиатуры).*/

#include <stdio.h>
#define SIZE 8

void binary_number(char a)
{  
    const unsigned int mask = 1;
    int array[SIZE] = {0};

    for (int i = SIZE - 1; i >= 0 ; i--)
    {
        array[i] = mask & a;
        a = a >> 1;
    }

    int i = 0;

    for (i; i < SIZE; i++)
    {
        if (array[i] == 0)
        {
            continue;
        }
        break;
    }

    printf("Given number in binary: ");
    
    for (i; i < SIZE; i++)
    {
        printf("%d", array[i]);
    } 
}

void counter_ones(char a)
{
    const unsigned int mask = 1;
    int array[SIZE] = {0};

    for (int i = SIZE - 1; i >= 0 ; i--)
    {
        array[i] = mask & a;
        a = a >> 1;
    }

    int counter = 0;
    
    for (int i = 0; i < SIZE; i++)
    {
        if (array[i] == 1)
        {
            counter++;
        }
    }

    printf("Number of 1's: %d", counter);
}

int main()
{
    char a = 0;

    printf("Insert decimal number: ");
    scanf("%hhd", &a);

    binary_number(a);
    printf("\n");
    counter_ones(a);
    printf("\n");

    return 0;
}
