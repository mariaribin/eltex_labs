#include <stdio.h>

#define SIZE 2

int main()
{
    printf("The size of matrix is %dx%d:\n", SIZE, SIZE);

    if (SIZE == 0)
    {
        printf("%d is an incorrect size of matrix.\nThe matrix size should be more than 0!\n", SIZE);
        return 0;
    }

    int array[SIZE][SIZE];

    int value = 1;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
           array[i][j] = value;
           value++;
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