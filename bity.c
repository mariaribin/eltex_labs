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

    for (i; i < 32; i++)
    {
        if (array[i] == 0)
        {
            continue;
        }
        break;
    }
    
    for (i; i < 32; i++)
    {
        printf("%d", array[i]); 
    }
    
    return 0;
}

int main()
{
    unsigned int a = 0;

    scanf("%u", &a);

    binary_number(a);

    return 0;
}