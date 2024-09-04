#include <stdio.h>
#include <stdbool.h>
#include "math.h"

int main()
{
    int Num_a = 0;
    int Num_b = 0;
    int menu = 0;
    bool run = true;

    while(run)
    {
        printf("Insert values A and B:\n");
        scanf("%d", &Num_a);
        scanf("%d", &Num_b);

        printf("\n1 - Addition (a - b)"
               "\n2 - Substraction (a + b)"
               "\n3 - Multiplication (a x b)"
               "\n4 - Division (a / b)\n\n");

        printf("Type command: ");
        scanf("%d", &menu);
        
        if (0 == Num_b && 4 == menu)
        {
            printf("\nError! Can't divide by 0!\n\n");
            continue;
        }

        switch(menu)
        {
            case 1:
            {
                printf("\n%d + %d = %d", Num_a, Num_b, add(Num_a, Num_b));
                break;
            }

            case 2:
            {
                printf("\n%d - %d = %d", Num_a, Num_b, sub(Num_a, Num_b));
                break;
            }

            case 3:
            {
                printf("\n%d * %d = %d", Num_a, Num_b, mult(Num_a, Num_b));
                break;
            }

            case 4:
            {
                printf("\n%d / %d = %d", Num_a, Num_b, div(Num_a, Num_b));
                break;
            }

            default:
            {
                printf("\nWrong command %d. Try again.\n", menu);
                break;
            }
        }
        printf("\n\n");
    }
    return 0;
}
