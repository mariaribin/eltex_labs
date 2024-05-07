/*Поменять в целом положительном числе (типа int) значение третьего
байта на введенное пользователем число (изначальное число также
вводится с клавиатуры) через указатель (не применяя битовые
операции).*/

#include <stdio.h>
#define SIZE 32

void binary_number(int num)
{
    const unsigned int mask = 1;
    int number[SIZE] = {0};

    for (int i = SIZE - 1; i >= 0 ; i--)
    {
        number[i] = mask & num;
        num = num >> 1;
    }

    printf("Given number in binary: ");

    for (int i = 0; i < SIZE; i++)
    {
        printf("%d", number[i]);
    }
    
    printf("\n");
}

int main()
{
    int num1 = 0;
    char num2 = 0;

    printf("Insert Number 1: ");
    scanf("%d", &num1);
    binary_number(num1);

    printf("Insert Number 2 (0...255): ");
    scanf("%hhd", &num2);

    char *ptr = &num1;

    ptr+=2;
    *ptr = num2; 

    printf("The 3d byte was changed.\n");
    binary_number(num1);
    printf("= %d in decimal", num1);
    printf("\n");

    return 0;
}
