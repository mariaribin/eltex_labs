/*Реализовать файловый менеджер на подобии mc (рисунок 1), с
функционалом навигации по папкам и двумя панелями (переключение
между панелями через Tab). Использовать для графики библиотеку
ncurses.*/

#include <stdio.h>
#include <ncurses.h>

enum Window_borders
{
    WINDOW_1 = 1,
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

    WINDOW * first_win = newwin(23, 40, 0, 1);
    box(first_win, 0, 0);

    refresh();
    wrefresh(first_win);

    keypad(first_win, true);

    sleep(3);
    endwin();
        
    printf("\n X = %d \n", xMax);
    printf("\n Y = %d \n", yMax);

    return 0;
}