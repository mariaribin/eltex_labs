#include <stdio.h>

int task1_hw2()
{
    int value = 1;
    int n = 0;

    printf("Define the size of matrix:\n");

    scanf("%d", &n);

    if (n <= 0)
    {
        printf("Impossible size of matrix\n");
        return 0;
    }


    int array[n][n];


    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
           array[i][j] = value;
           value++;
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }

    return 0;
}

int task2_hw2()
{
    int n = 0;

    printf("Define the size of array:\n");
    
    scanf("%d", &n);

    if (n <= 0)
    {
        printf("Impossible size of matrix\n");
        return 0;
    }

    int array[n];

    printf("Insert the values of array:\n");

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &array[i]);

    }

    
    int array1[n];
    int j = n - 1;

    for (int i = 0; i < n; i++)
    {
        array1[j] = array[i];
        j--;
    }

    for (int i = 0; i < n; i++)
    {
        printf("%d ", array1[i]);
    }

    return 0;

}

int task4_hw2()
{
    int n = 0;

    printf("Define the size of matrix:\n");

    scanf("%d", &n);

    if (n <= 0)
    {
        printf("Impossible size of matrix\n");
        return 0;
    }

    int array[n][n];
    
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
           array[i][j] = 0;
        }
    }

    int counter = 1;
    int square = n * n;
    int limit = n;
    int i = 0;
    int j = 0;
    int shift = 0;
    
    while (counter <= square) 
    {
        for (j = 0 + shift; j < limit; j++)
        {
            array[shift][j] = counter;
            counter++;
        }
        
        for (i = shift + 1; i < limit; i++)
        {
            array[i][limit - 1] = counter;
            counter++;
        }

        for (j = limit - 2; j >= shift; j--)
        {
            array[limit - 1][j] = counter;
            counter++;
        }

        for (i = limit - 2; i > shift; i--)
        {   
            array[i][shift] = counter;
            counter++;
        }
 
        limit--;
        shift++;
    }

   for (int i1 = 0; i1 < n; i1++)
    {
        for (int j1 = 0; j1 < n; j1++)
        {
            printf("%d ", array[i1][j1]);
        }
        printf("\n");
    }

    return 0;

}

int task3_hw2()
{
   int n = 0;

    printf("Define the size of matrix:\n");

    scanf("%d", &n);

    if (n <= 0)
    {
        printf("Impossible size of matrix\n");
        return 0;
    }

    int array[n][n]; 

    int limit = n;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < limit; j++)
        {
            array[i][j] = 1;
        }

        for (int j = n; j >= limit; j--)
        {
            array[i][j] = 0;
        }

        limit--;
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }

    return 0;
}