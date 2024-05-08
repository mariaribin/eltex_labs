/*Поменять в целом положительном числе (типа int) значение третьего
байта на введенное пользователем число (изначальное число также
вводится с клавиатуры)*/

#include <stdio.h>
#define SIZE1 32
#define SIZE2 8

/*the function calculates the power of two,
which is passed by value*/
int power_of_two(char const power)
{
    int multiply = 1;
    
    for (int i = 0; i < power; i++)
    {
        multiply = multiply * 2;
    }

    return multiply;
}

/*the function takes two values 
and changes the 3rd byte of value 1 to value 2*/
void change_third_byte(unsigned int a, unsigned char b)
{
    const unsigned int mask = 1;
    int number_1[SIZE1] = {0};
    int number_2[SIZE2] = {0};

    for (int i = SIZE1 - 1; i >= 0 ; i--)
    {
        number_1[i] = mask & a;
        a = a >> 1;
    }

    printf("Number 1 in binary: ");

    for (int i = 0; i < SIZE1; i++)
    {
        printf("%d", number_1[i]);
    }

    printf("\n");

    for (int i = SIZE2 - 1; i >= 0 ; i--)
    {
        number_2[i] = mask & b;
        b = b >> 1;
    }

    printf("Number 2 in binary: ");

    for (int i = 0; i < SIZE2; i++)
    {
        printf("%d", number_2[i]);
    }
    
    printf("\n");

    // the 3rd byte of value 1 is changed
    int j = 0;

    for (int i = SIZE1 - 24; i < SIZE1 - 16; i++)
    {
        number_1[i] = number_2[j];
        j++;
    }
  
    printf("Binary Number 1 with CHANGED 3rd byte: ");
    
    for (int i = 0; i < SIZE1; i++)
    {
        printf("%d", number_1[i]);
    }

    printf(" ");

    unsigned int decimal_number = 0;
    char counter = SIZE1 - 1;

    // converting changed value 1 from binary to decimal
    for (int i = 0; i < SIZE1; i++)
    {    
        if (number_1[i] == 1)
        {
            decimal_number = decimal_number + power_of_two(counter);
        }
        
        counter--;
    }
    
    printf("(= %u decimal)", decimal_number);
}

int main()
{
    unsigned int num1 = 0;
    unsigned char num2 = 0;

    printf("Insert Number 1: ");
    scanf("%u", &num1);
    printf("Insert Number 2 (0...255): ");
    scanf("%hhd", &num2);

    change_third_byte(num1, num2);
    printf("\n");

    return 0;
}
