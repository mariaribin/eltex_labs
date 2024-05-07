/*Вывести заданный массив размером N в обратном порядке.*/

#include <stdio.h>
#define SIZE 4

int main()
{
    printf("The size of aray is %d.\n", SIZE);

    int array1[SIZE] = {0};

    printf("\nInsert the values of array:\n");

    for (int i = 0; i < SIZE; i++)
    {
        scanf("%d", &array1[i]);
    }

    int array2[SIZE] = {0};
    int j = SIZE - 1;

    /*we fill the new array with values of given array,
    starting from the end*/
    for (int i = 0; i < SIZE; i++)
    {
        array2[j] = array1[i];
        j--;
    }

    printf("\nThe given array, inverted:\n");

    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", array2[i]);
    }

    printf("\n");

    return 0;
}
