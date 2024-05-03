#include <stdio.h>

#define SIZE 32

/*the function calculates the power of two, 
which is passed by value*/
int power_of_two(char power) 
{
    int mult = 1;
    
    for (int i = 0; i < power; i++)
    {
        mult = mult * 2;
    }

    return mult;
}

/*the function takes two values 
and changes the 3rd byte of value 1 to value 2*/
void binary_number(unsigned int a, unsigned char b) 
{
    const unsigned int mask = 1;
    
    int array[SIZE] = {0};
    int array2[8] = {0};
    
    for (int i = SIZE - 1; i >= 0 ; i--) 
    {
        array[i] = mask & a;
        a = a >> 1;
    }

    printf("Number 1 in binary: "); 

    for (int i = 0; i < SIZE; i++)
    {
        printf("%d", array[i]); 
    }

    printf("\n");

    for (int i = 7; i >= 0 ; i--) 
    {
        array2[i] = mask & b;
        b = b >> 1;
    }

    printf("Number 2 in binary: "); 

    for (int i = 0; i < 8; i++)
    {
        printf("%d", array2[i]); 
    }
    
    printf("\n");

    // the 3rd byte of value 1 is changed
    int j = 0;

    for (int i = 8; i < 16; i++) 
    {
         array[i] = array2[j];
         j++;
    }
  
    printf("Binary Number 1 with CHANGED 3rd byte: "); 
    
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d", array[i]); 
    }

    printf(" ");

    unsigned int decimal_number = 0;
    char counter = 31;

    // converting changed value 1 from binary to decimal
    for (int i = 0; i < SIZE; i++) 
    {
        
        if (array[i] == 1)
        {
            decimal_number = decimal_number + power_of_two(counter);
            
        }
        
        counter--;
    }
    
    printf("(= %u decimal)", decimal_number); 
}

int main()
{
    unsigned int a = 0;
    unsigned char b = 0;

    printf("Insert Number 1: ");
    scanf("%u", &a);
    printf("Insert Number 2 (0...255): ");
    scanf("%hhd", &b);

    binary_number(a, b);
    printf("\n");

    return 0;
}