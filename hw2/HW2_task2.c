#include <stdio.h>

#define SIZE 4

int main()
{
    printf("The size of aray is %d.\n", SIZE);

    if (0 == SIZE)
    {
        printf("%d is an incorrect size of matrix.\nThe matrix size should be more than 0!\n", SIZE);

        return 0;
    }

    int array[SIZE] = {0};

    printf("\nInsert the values of array:\n");

    for (int i = 0; i < SIZE; i++)
    {
        scanf("%d", &array[i]);

    }

    int array1[SIZE] = {0};
    int j = SIZE - 1;

    /*we fill the new array with values of given array, 
    starting from the end*/
    for (int i = 0; i < SIZE; i++)
    {
        array1[j] = array[i];
        j--;
    }

    printf("\nThe given array, inverted:\n");

    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", array1[i]);
    }

    printf("\n");

    return 0;
}