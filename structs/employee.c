#include <stdio.h>

#define SIZE 4

struct Employee
{
    char name[20];
    char surname[20];
    char patronymic[20];
    char position[40];
    int salary;
    char info[1000];

};

void set_employee(struct Employee *employee)
{
    printf("Name: ");
    scanf("%s", employee->name);

    printf("Surname: ");
    scanf("%s", employee->surname);

    printf("Patronymic: ");
    scanf("%s", employee->patronymic);

    printf("Position: ");
    scanf("%s", employee->position);

    printf("Salary: ");
    scanf("%d", &employee->salary);

    sprintf(employee->info, "Name: %s %s %s | Position: %s | Salary: %d", 
    employee->name, employee->surname, employee->patronymic, employee->position, employee->salary);

}

void print_employee(struct Employee *employee)
{
    for(int i = 0; i < SIZE; i++)
    {
        
        printf("%s\n", employee[i].info);
        printf("\n");
    }
}

void salary_below_median(struct Employee *employee)
{
    int sum = 0;

    for(int i = 0; i < SIZE; i++)
    {
        sum = employee[i].salary + sum;
    }

    int median = sum / SIZE;

    for(int i = 0; i < SIZE; i++)
    {
        if(employee[i].salary < median)
        {
            printf("%s\n", employee[i].info);
        }
    }
}

int main()
{
    struct Employee employee[SIZE] = {0};

    int counter = 1;
    
    for(int i = 0; i < SIZE; i++)
    {
        printf("Employee %d:\n", counter);
        set_employee(&employee[i]);
        printf("\n");
        counter++;
    }

    printf("\n");
    printf("All employees:\n");

    print_employee(&employee);

    printf("\n");
    printf("\n");
    printf("Employees with salaries below median:\n");

    salary_below_median(&employee);

    return 0;
}