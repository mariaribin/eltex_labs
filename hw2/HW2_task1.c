/*Вывести квадратную матрицу по заданному N.*/

#include <stdio.h>
#define SIZE 3

int main()
{
    printf("The size of matrix is %dx%d:\n", SIZE, SIZE);

    int array[SIZE][SIZE] = {0};
    int counter = 1;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
           array[i][j] = counter;
           counter++;
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }

    return 0;
}
