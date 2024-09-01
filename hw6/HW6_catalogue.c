/*Переписать справочник абонентов (задание на структуры) со статического
массива структур на динамический. При запуске программы массив пуст, при
добавлении первого абонента выделяется память только под один элемент. По
мере добавления абонентов массив увеличивается по одному элементу. При
удалении, элемент, который удаляется, перемещается в конец массива и размер
массива уменьшается на один элемент. По завершению программы память под
массив высвобождается. В остальном функционал, как и в предыдущем
задании.*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdlib.h>
#define SIZE_NAME 15
#define SIZE_TEL_NUM 15
#define SIZE_INFO 100

struct Abonent
{
    char name[SIZE_NAME];
    char surname[SIZE_NAME];
    char tel[SIZE_TEL_NUM];
    char info[SIZE_INFO];
    int index;
    bool exist;
};

void add_abonent(struct Abonent *abonent, int counter)
{
    printf("Name: ");
    scanf("%s", abonent->name);

    printf("Surname: ");
    scanf("%s", abonent->surname);

    printf("Tel. number: ");
    scanf("%s", abonent->tel);

    snprintf(abonent->info, sizeof(abonent->info), "Name: %s %s | Tel.number: %s",
    abonent->name, abonent->surname, abonent->tel);

    abonent->exist = true;

}

void print_all_abonents(struct Abonent *abonent, int *counter)
{
    int numeration = 1;

    printf("All subscribers:\n");
    
    for (int i = 0; i < *counter; i++)
    {
        if (abonent[i].exist)
        {
            printf("%d) %s (index %d)\n", numeration, abonent[i].info,
            abonent[i].index);
            numeration++;
        }
    }
}

void delete_abonent(struct Abonent *abonent, int *counter)
{
    int number = 0;
    int deleted_slot = 0;

    printf("Choose the slot to delete.\n\n");
    print_all_abonents(abonent, counter);
    printf("\nType the INDEX of slot to delete: ");
    scanf("%d", &number);

    for (int i = 0; i < *counter; i++)
    {
        if (number == abonent[i].index)
        {
            memset(abonent[i].info, 0, sizeof(abonent[i].info));
            memset(abonent[i].name, 0, sizeof(abonent[i].name));
            memset(abonent[i].surname, 0, sizeof(abonent[i].surname));
            memset(abonent[i].tel, 0, sizeof(abonent[i].tel));

            abonent[i].exist = false;
            break;
        }
        deleted_slot++;
    }

    if (deleted_slot < *counter)
    {
        for (int i = deleted_slot; i < *counter; i++)
        {
            abonent[i] = abonent[i + 1];
        }
    }

    printf ("\n");
    *counter = *counter - 1;
}

void search_abonent(struct Abonent *abonent, int counter)
{
    char search_abonent[SIZE_NAME] = {0};
    bool found = false;

    printf("Enter name: ");
    scanf("%s", search_abonent);
    printf("\nMatching subscribers:\n\n");

    for (int i = 0; i < counter; i++)
    {
        int result = strcmp(search_abonent, abonent[i].name);

        if (0 == result)
        {
            printf("%s\n", abonent[i].info);
            found = true;
        }
    }

    if (found == false)
    {
        printf("no mahtching subscribers.\n");
    }
}

int main()
{
    struct Abonent *abonent = NULL;
    int menu = 0;
    int counter = 0;
    int i = 0;
    int check_memory = 0;
    bool run = true;

    printf("Type 1-5 to:\n1 - Add subscriber.\n"
        "2 - Delete subscriber.\n3 - Find subscribers by name.\n"
        "4 - Show all.\n5 - Exit.\n");

    while (run)
    {
        printf("\nType command 1-5 (6 - to show menu): ");
        scanf("%d", &menu);

        if (menu == 0)
        {
            printf("\nWrong command %d. Try again.\n", menu);
            break;
        }

        switch (menu)
        {
            case 1: 
            {
                if (0 == counter)
                {
                    abonent = calloc(1, sizeof(struct Abonent));
                    if(NULL == abonent)
                    {
                        printf("calloc() failed.\n");
                        break;
                    }

                    add_abonent(abonent, counter);
                  
                    counter++;

                    for (int i = 0; i < counter; i++)
                    {
                        abonent[i].index = i + 1;
                    }

                    check_memory = sizeof(struct Abonent) * (counter);
                    printf("\nAllocated memory = %d bytes\n\n", check_memory);
                    break;
                }
                else
                {
                    abonent = realloc(abonent, sizeof(struct Abonent) * (counter + 1));
                    if(NULL == abonent)
                    {
                        printf("realloc() failed.\n");
                        break;
                    }
                    
                    add_abonent(abonent + counter, counter);
                     
                    counter++;

                    for (int i = 0; i < counter; i++)
                    {
                        abonent[i].index = i + 1;
                    }

                    check_memory = sizeof(struct Abonent) * (counter);
                    printf("\nAllocated memory = %d bytes\n\n", check_memory);
                }
                break;
            }

            case 2:
            {
                printf("\n");

                if (0  == counter)
                {
                    printf("Nothing to delete!\n");
                    break;
                }
                delete_abonent(abonent, &counter);

                if (counter == 0)
                {
                    abonent = realloc(abonent, sizeof(struct Abonent) * (counter + 1));
                    check_memory = sizeof(struct Abonent) * (counter + 1);
                    printf("\nAllocated memory = %d bytes (reserved)\n\n", check_memory);
                }
                else
                {
                    abonent = realloc(abonent, sizeof(struct Abonent) * counter); 
                    check_memory = sizeof(struct Abonent) * (counter);
                    printf("\nAllocated memory = %d bytes\n\n", check_memory);
                }
                
                if(NULL == abonent)
                {
                    printf("realloc() failed.\n");
                    break;
                }

                for (int i = 0; i < counter; i++)
                {
                    abonent[i].index = i + 1;
                }       
                
                break;
            }
            
            case 3:
            {
                printf("\n");

                if (0  == counter)
                {
                    printf("No subscribers.\n");
                    break;
                }

                search_abonent(abonent, counter);
                break;
            }
        
            case 4:
            {
                printf("\n");

                if (0  == counter)
                {
                    printf("No subscribers.\n");
                    break;
                }

                print_all_abonents(abonent, &counter);
                break;
            }

            case 5:
            {
                free(abonent);
                return 0;
            }

            case 6:
            {
                printf("\n1 - Add subscriber.\n2 - Delete subscriber.\n"
                    "3 - Find subscribers by name.\n4 - Show all.\n5 - Exit.\n");
                break;
            }

            default:
            {
                printf("\nWrong command %d. Try again.\n", menu);
                break;
            }
        }
    }

return 0;
}
