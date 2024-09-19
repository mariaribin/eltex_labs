/*Реализовать файловый менеджер на подобии mc (рисунок 1), с
функционалом навигации по папкам и двумя панелями (переключение
между панелями через Tab). Использовать для графики библиотеку
ncurses.*/

#include <stdio.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>


int main()
{
    initscr();
    bool run = true;
    int menu = 0;
    
    int ret = mkdir("./dir1", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == ret)
    {
        perror("make directory failed");
        return -1;
    }

    ret = mkdir("./dir2", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == ret)
    {
        perror("make directory failed");
        return -1;
    }

    ret = mkdir("./dir3", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    {
        perror("make directory failed");
        return -1;
    }

    while(run);
    {
        getch(menu);
        printf(\n);

        switch(menu):

            case KEY_DOWN:
            {
                keypad(stdscr, true);
            }

            case KEY_UP:
            {
                keypad(stdscr, true);
            }

            case KEY_LEFT:
            {
                keypad(stdscr, true);
            }

            case KEY_RIGHT:
            {
                keypad(stdscr, true);
            }

    }



    /*DIR *fd = opendir("./dir1");
    if (!fd)
    {
        perror("open directory failed");
        return -1;
    }*/

    endwin();
    return 0;
}