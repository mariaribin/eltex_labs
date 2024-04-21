#include <stdio.h>

#define SIZE 32

int power_of_two(char power) // функция вычисляет степень двойки, которая передается по значению
{
    int mult = 1;
    
    for (int i = 0; i < power; i++)
    {
        mult = mult * 2;
    }

    return mult;
}

void binary_number(unsigned int a, unsigned char b) // функция принимает два значения и заменяет третий байт числа 1 на число 2
{
    
    const unsigned int mask = 1;
    
    int array[SIZE] = {0};
    int array2[8] = {0};
    
    for (int i = SIZE - 1; i >= 0 ; i--) // запись в массив 1 бинарного представления числа 1
    {
        array[i] = mask & a;
        a = a >> 1;
    }

    printf("Binary number 1: "); // вывод числа 1 в бинарном представлении

    for (int i = 0; i < SIZE; i++)
    {
        printf("%d", array[i]); 
    }

    printf("\n");

    for (int i = 7; i >= 0 ; i--) // запись в массив 2 бинарного представления числа 2
    {
        array2[i] = mask & b;
        b = b >> 1;
    }

    int i = 0;

    printf("Binary number 2: "); // вывод числа 2 в бинарном представлении

    for (int i = 0; i < 8; i++)
    {
        printf("%d", array2[i]); 
    }
    
    printf("\n");

    for (int i = 0; i < 8; i++) // замена третьего байта числа 1
    {
         array[i] = array2[i];
    }

    printf("Binary number 1 with CHANGED 3rd byte: "); // вывод измененного числа 1 в бинарном представлении
    
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d", array[i]); 
    }

    printf(" ");

    unsigned int decimal_number = 0;
    char counter = 31;

    for (int i = 0; i < SIZE; i++) // перевод измененного числа 1 из двоичной системы в десятичную
    {
        
        if (array[i] == 1)
        {
            decimal_number = decimal_number + power_of_two(counter);
            
        }
        
        counter--;
    }
    
    printf("(= %u decimal)", decimal_number); // вывод десятичной формы измененного числа 1
}

int main()
{
    unsigned int a = 0;
    unsigned char b = 0;

    printf("Insert number 1: ");
    scanf("%u", &a);
    printf("Insert number 2 (0...255): ");
    scanf("%hhd", &b);

    binary_number(a, b);
    printf("\n");

    return 0;
}