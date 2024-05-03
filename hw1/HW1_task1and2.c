#include <stdio.h>

int binary_number(unsigned int a) 
{
    const unsigned int mask = 1;
    
    int array[32] = {0};

    for (int i = 31; i >= 0 ; i--)
    {
        array[i] = mask & a;
        a = a >> 1;
    }

    int i = 0;

    /*to make sure the first 0's aren't printed, 
    we skip them with a FOR cycle*/
    for (i; i < 32; i++)
    {
        if (array[i] == 0)
        {
            continue;
        }
        break;
    }
    
    /*once the program reaches the first meaningful number, 
    the program starts printing it and the rest of numbers in the console*/
    for (i; i < 32; i++)
    {
        printf("%d", array[i]); 
    }
    
    return 0;
}

int main()
{
    unsigned int a = 0;

    printf("Insert a decimal number: ");
    scanf("%u", &a);
    printf("\nGiven number in binary: ");
    binary_number(a);
    printf("\n");

    return 0;
}