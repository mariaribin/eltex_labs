/*Реализовать файловый менеджер на подобии mc (рисунок 1), с
функционалом навигации по папкам и двумя панелями (переключение
между панелями через Tab). Использовать для графики библиотеку
ncurses.*/

#include <stdio.h>
#include <ncurses.h>
#include <dirent.h>

enum Window_borders
{
    WINDOW_HEIGHT = 23,
    WINDOW_WIDTH = 40,
    WINDOW_TOP_BORDER = 0,
    WINDOW_SIDE_BORDER = 1,
};

int main()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    int yMax = 0;
    int xMax = 0;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW * first_win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_TOP_BORDER, WINDOW_SIDE_BORDER);
    box(first_win, 0, 0);

    refresh();
    wrefresh(first_win);

    keypad(first_win, true);

    bool run = true;

    while(run)
    {

        int val = wgetch(first_win);
        switch(val)
        {
            case KEY_UP:
            {   
                run = false;
                break;
            }
            case KEY_DOWN:
            {   
                run = false;
                break;
            }
        }
    }

    endwin();

    struct dirent **namelist = NULL;
    char path[32] = "./";
        
    int count = scandir(path, &namelist, 0, alphasort);
    if (-1 == count)
    {
        perror("scandir");
    }

    printf("\n count = %d\n", count);

    for (int i = 0; i < count; i++)
    {
        printf("%s\n", namelist[i]->d_name);
    }

    return 0;
}
