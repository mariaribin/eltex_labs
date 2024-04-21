#include <stdio.h>

int main()
{

    int array[3][3] =
        {
            {1, 5, 6},
            {3, 2, 1},
            {6, 6, 6}
        };

   /* int array2[3][3] = {0};

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            scanf("%d", &array2[i][j]);
        }
    }*/

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            printf("%d ",array[i][j]);
        }
        printf("\n");
    }

    return 0;
}