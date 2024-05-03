#include <stdio.h>

#define SIZE 8

int main()
{
    printf("The size of matrix is %dx%d:\n", SIZE, SIZE);

    if (0 == SIZE)
    {
        printf("%d is an incorrect size of matrix.\nThe matrix size should be more than 0!\n", SIZE);
        return 0;
    }

    int array[SIZE][SIZE] = {0};
    
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
           array[i][j] = 0;
        }
    }

    int counter = 1;
    int square = SIZE * SIZE;
    int limit = SIZE; //size limit for directions LEFT-->RIGHT and UP-->DOWN
    int shift = 0; //size limit for directions RIGHT-->LEFT and DOWN-->UP
    int i = 0;
    int j = 0;
    
    /*We fill the matrix going in spiral, in 4 cycles for 4 directions: 
    left to right --> top to down --> right to left --> down to top. 
    Until we reach the number of square. 
    Limit for each direction is smaller by 1 at every iteration.*/    
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

   for (int i1 = 0; i1 < SIZE; i1++)
    {
        for (int j1 = 0; j1 < SIZE; j1++)
        {
            if (array[i1][j1] < 10)
            {
                printf(" %0d ", array[i1][j1]);
            }
            else
            {
                printf("%d ", array[i1][j1]);
            }
        }
        printf("\n");
    }

    return 0;
}