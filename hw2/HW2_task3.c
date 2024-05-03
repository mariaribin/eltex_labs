#include <stdio.h>

#define SIZE 10

int main()
{
    printf("The size of matrix is %dx%d:\n", SIZE, SIZE);

    if (0 == SIZE)
    {
        printf("%d is an incorrect size of matrix.\nThe matrix size should be more than 0!\n", SIZE);
        return 0;
    }

    int array[SIZE][SIZE]; 

    int limit = SIZE;

    /*here we fill each row with 1's (from left to right -->)
    and with 0's (from right to left <--)*/
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < limit; j++) 
        {
            array[i][j] = 1;
        }

        for (int j = SIZE; j >= limit; j--)
        {
            array[i][j] = 0;
        }

        limit--;
    }

    printf("\n");

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