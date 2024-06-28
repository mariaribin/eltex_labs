/*Написать программу абонентский справочник. Список абонентов
представляет собой статический массив (100 элементов)

При запуске на экран выводится текстовое меню:
1) Добавить абонента
2) Удалить абонента
3) Поиск абонентов по имени
4) Вывод всех записей
5) Выход
и пользователю предлагается ввести пункт меню с клавиатуры. Добавление
абонента в массив реализуется простым заполнением свободной структуры,
при выходе за 100 абонентов уведомить пользователя о переполнении
справочника и не позволять больше добавлять абонентов. При удалении
структура заполняется нулями. При поиске пользователь вводит с клавиатуры
имя абонентов и на экран выводится список всех абонентов с таким же именем.
Программа продолжает выполняться пока пользователь не введет пункт 5.*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define SIZE_CATALOGUE 100
#define SIZE_NAME 15
#define SIZE_TEL_NUM 15
#define SIZE_INFO 100

struct Abonent
{
    char name[SIZE_NAME];
    char surname[SIZE_NAME];
    char tel[SIZE_TEL_NUM];
    char info[SIZE_INFO];
    char index;
    bool exist;
};

void add_abonent(struct Abonent *abonent)
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

void print_all_abonents(struct Abonent *abonent)
{
    char counter = 1;

    printf("All subscribers:\n");
    
    for (int i = 0; i < SIZE_CATALOGUE; i++)
    {
        if (abonent[i].exist)
        {
            printf("%d) %s (index %d)\n", counter, abonent[i].info,
            abonent[i].index);

            counter++;
        }
    }
}

void delete_abonent(struct Abonent *abonent)
{
    char number = 0;

    printf("Choose the slot to delete.\n\n");
    print_all_abonents(abonent);
    printf("\nType the INDEX of slot to delete: ");

    scanf("%hhd", &number);

    for (int i = 0; i < SIZE_CATALOGUE; i++)
    {
        if (number == abonent[i].index)
        {
            memset(abonent[i].info, 0, sizeof(abonent[i].info));
            memset(abonent[i].name, 0, sizeof(abonent[i].name));
            memset(abonent[i].surname, 0, sizeof(abonent[i].surname));
            memset(abonent[i].tel, 0, sizeof(abonent[i].tel));

            abonent[i].exist = false;
        }
    }
}

void search_abonent(struct Abonent *abonent)
{
    char search_abonent[SIZE_NAME] = {0};

    printf("Enter name: ");
    scanf("%s", search_abonent);
    printf("\nMatching subscribers:\n\n");

    for (int i = 0; i < SIZE_CATALOGUE; i++)
    {
        int result = strcmp(search_abonent, abonent[i].name);

        if (0 == result)
        {
            printf("%s\n", abonent[i].info);
        }
    }
}

int main()
{
    struct Abonent abonent[SIZE_CATALOGUE] = {0};
    char menu = 0;
    bool run = true;
    int counter = 0;
    int i = 0;

    /*setting the index for every slot in the catalogue 
    to later use them in "print" and "delete" functions*/
    for (int j = 0; j < SIZE_CATALOGUE; j++) 
    {
        abonent[j].index = j + 1;
    }

    printf("Type 1-5 to:\n1 - Add subscriber.\n"
        "2 - Delete subscriber.\n3 - Find subscribers by name.\n"
        "4 - Show all.\n5 - Exit.\n");

    while (run)
    {
        printf("\nType command 1-5 (6 - to show menu): ");
        scanf("%hhd", &menu);

        switch (menu)
        {
            case 1: 
            {
                if (counter >= SIZE_CATALOGUE)
                {
                    printf("You've run out of free slots. "
                    "Clear slots to add new subscribers.\n");

                    break;
                }
                else
                {
                    for (int j = 0; j < SIZE_CATALOGUE; j++)
                    {
                        if (abonent[j].exist == false)
                        {
                            add_abonent(&abonent[j]);
                            counter++;
                            break;
                        }
                    }
                }

                break;
            }

            case 2:
            {
                printf("\n");
                delete_abonent(abonent);

                if (counter > 0)
                {
                    counter--;
                }

                break;
            }
            
            case 3:
            {
                printf("\n");
                search_abonent(abonent);
                break;
            }
        
            case 4:
            {
                printf("\n");
                print_all_abonents(abonent);
                break;
            }

            case 5:
            {
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
                printf("\nWrong command. Try again.\n");
                break;
            }
        }
    }

return 0;
}
