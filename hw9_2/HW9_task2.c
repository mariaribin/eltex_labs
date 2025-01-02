/*Реализовать файловый менеджер на подобии mc (рисунок 1), с
функционалом навигации по папкам и двумя панелями (переключение
между панелями через Tab). Использовать для графики библиотеку
ncurses.*/

#include <stdio.h>
#include <ncurses.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define EMPTY_STR "                                 "

struct Getfiles
{
    struct dirent **namelist;
    int size;
};

enum Window_borders
{
    WINDOW_HEIGHT = 30,
    WINDOW_WIDTH = 40,
    WINDOW_TOP_BORDER = 0,
    WINDOW_SIDE_BORDER = 1,
};

void get_files(struct Getfiles *ptr, char *path)
{      
    int count = scandir(path, &ptr->namelist, 0, alphasort);
    if (-1 == count)
    {
        perror("scandir");
    }

    ptr->size = count;
}

void free_files(WINDOW * menuwin, struct Getfiles *file)
{
    for (int i = 0; i < file->size; i++)
    {
        mvwprintw(menuwin, i + 1, 1, EMPTY_STR);
    }
    
    while(file->size--)
    {
        if (file->namelist[file->size])
        {
            free(file->namelist[file->size]);
            file->namelist[file->size] = NULL;
        }
    }

    if (file->namelist)
    {
        free(file->namelist);
        file->namelist = NULL;
    }

    file->size = 0;
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
        struct Getfiles getfile = {0};
        get_files(&getfile, "./");  

        for (int i = 0; i < getfile.size; i++)
        {
            if (i == highlight)
            {
                wattron(first_win, A_REVERSE);
            }
            
            char buf[32] = "/";
            if (getfile.namelist[i]->d_type != 4)
            {
                buf[1] = '.';
            }

            strncat(buf, getfile.namelist[i]->d_name, 32);
            mvwprintw(first_win, i + 1, 1, buf);
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
            case 10:
            {
                if (getfile.namelist[highlight]->d_type != 4)
                {
                    break;
                }

                if (0 == strcmp(getfile.namelist[highlight]->d_name, "."))
                {
                    break;
                }
                
                char pwd[1024] = {0};
                getcwd(pwd, 1024);
                
                strcat(pwd, "/");
                strncat(pwd, getfile.namelist[highlight]->d_name, 1024);
                chdir(pwd);
                
                free_files(first_win, &getfile);
                get_files(&getfile, pwd);
                highlight = 0;
                break;
            }
        }
    }

    endwin();
    return 0;
}
