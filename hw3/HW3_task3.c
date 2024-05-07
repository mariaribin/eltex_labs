/*Напишите программу, в которой определите массив типа int (размер
массива 10 элементов от 1 до 10). Используйте указатель для вывода
элементов массива на консоль (применять можно только арифметику
указателей, а не индексы).*/

#include <stdio.h>
#define SIZE 10

int main()
{
    int array[SIZE] = {0};
    int *p_array = &array[0];
    int counter = 1;

    for (int i = 0; i < SIZE; i++)
    {
        array[i] = counter;
        counter++;
    }

    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", *p_array);
        p_array++;
    }

    printf("\n");

    return 0;
}
