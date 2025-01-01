/*Реализовать файловый менеджер на подобии mc (рисунок 1), с
функционалом навигации по папкам и двумя панелями (переключение
между панелями через Tab). Использовать для графики библиотеку
ncurses.*/

#include <stdio.h>
#include <ncurses.h>
#include <dirent.h>

struct get_files
{
    struct dirent **namelist;
    int size;
};

enum Window_borders
{
    WINDOW_HEIGHT = 23,
    WINDOW_WIDTH = 40,
    WINDOW_TOP_BORDER = 0,
    WINDOW_SIDE_BORDER = 1,
};

void get_files(struct get_files *ptr, char *path)
{      
    int count = scandir(path, &ptr->namelist, 0, alphasort);
    if (-1 == count)
    {
        perror("scandir");
    }

    ptr->size = count;
}

int main()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    int yMax = 0;
    int xMax = 0;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *first_win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_TOP_BORDER, WINDOW_SIDE_BORDER);
    box(first_win, 0, 0);

    refresh();
    wrefresh(first_win);

    keypad(first_win, true);

    
    int highlight = 0;

    while(1)
    {
        struct get_files getfile = {0};
        get_files(&getfile, "./");  

        for (int i = 0; i < getfile.size; i++)
        {
            if (i == highlight)
            {
                wattron(first_win, A_REVERSE);
            }
 
            mvwprintw(first_win, i + 1, 1, getfile.namelist[i]->d_name);
            wattroff(first_win, A_REVERSE);
        }

        wrefresh(first_win);
        
        int val = wgetch(first_win);
        switch(val)
        {
            case KEY_UP:
            {   
                highlight--;
                if (highlight < 0)
                {
                    highlight = 0;
                }
                break;
            }
            case KEY_DOWN:
            {   
                highlight++;
                if (highlight >= getfile.size)
                {
                    highlight = getfile.size - 1;
                }
                break;
            }
        }
    }

    endwin();

    

    

    return 0;
}
