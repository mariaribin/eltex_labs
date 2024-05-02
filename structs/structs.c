#include <stdio.h>


struct Person
{
    char name[20];
    char surname[20];
    int age;
    char info[100];
};

void set_person(struct Person *person)
{
    printf("Insert name: ");
    scanf("%s", person->name);

    printf("Insert surname: ");
    scanf("%s", person->surname);

    printf("Insert age: ");
    scanf("%d", &person->age);

    sprintf(person->info, "Name: %s Surname: %s Age: %d", person->name, person->surname, person->age);
}

int main()
{
    struct Person person[3] = {0};

    for(int i = 0; i < 3; i++)
    {
        set_person(&person[i]);
    }



    printf("\n");

    for(int i = 0; i < 3; i++)
    {
        printf("%s\n", person[i].info);
    }

    return 0;
}
