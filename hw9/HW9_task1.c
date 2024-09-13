/*Написать программу, которая создает файл с именем output.txt,
записывает в него строку “String from file”, затем считывает ее из файла
с конца и выводит на экран*/

#include <stdio.h>
#define SIZE 16

int main()
{
    char message[SIZE] = "String from file";
    int length = sizeof(message);

    FILE *file = fopen("output.txt", "w+");

    if(!file)
    {
        perror("open failed");
        return -1;
    }

    for (int i = 0; i < length; i++)
    {
        putc(message[i], file);
    }

    char symbol = 0;
    int pos = sizeof(message) - 1;

    while (pos >= 0)
    {
        fseek(file, pos, SEEK_SET);
        symbol = getc(file);
        printf("%c", symbol);
        pos--;
    }

    printf("\n");

    fclose(file);
    return 0;
}
