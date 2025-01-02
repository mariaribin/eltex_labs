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

#define EMPTY_STR "                                      "

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

    WINDOW_2_TOP_BORDER = 0,
    WINDOW_2_SIDE_BORDER = WINDOW_SIDE_BORDER + WINDOW_WIDTH,
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
    for (int i = 1; i < WINDOW_HEIGHT - 2; i++)
    {
        mvwprintw(menuwin, i + 1, 1, EMPTY_STR);
    }
    
    for (int i = 0; i < file->size; i++)
    {
        free(file->namelist[i]);
        file->namelist[i] = NULL;
    }

    free(file->namelist);
    file->namelist = NULL;
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

    WINDOW *first_win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 
                               WINDOW_TOP_BORDER, WINDOW_SIDE_BORDER);
    box(first_win, 0, 0);

    WINDOW *sec_win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 
                             WINDOW_2_TOP_BORDER, WINDOW_2_SIDE_BORDER);
    box(sec_win, 0, 0);

    WINDOW *ptr = first_win;

    refresh();
    wrefresh(first_win);
    wrefresh(sec_win);

    keypad(first_win, true);
    keypad(sec_win, true);

    int highlight1 = 0;
    int highlight2 = 0;
    int *p_highlight = &highlight1;
    char pwd1[1024] = {0};
    char pwd2[1024] = {0};
    char *p_pwd = pwd1;
    bool check = false;

    getcwd(pwd1, 1024);
    getcwd(pwd2, 1024);

    struct Getfiles getfile = {0};
    get_files(&getfile, p_pwd);

    for (int i = 0; i < getfile.size; i++)
    {
        if (i == WINDOW_HEIGHT - 2)
        {
            break;
        }
        
        char buf[256] = "/";
        if (getfile.namelist[i]->d_type != 4)
        {
            buf[1] = '.';
        }

        strncat(buf, getfile.namelist[i]->d_name, WINDOW_WIDTH - 4);
        mvwprintw(sec_win, i + 1, 1, buf);
    }

    wrefresh(sec_win);

    while(1)
    {
        for (int i = 0; i < getfile.size; i++)
        {
            if (i == WINDOW_HEIGHT - 2)
            {
                break;
            }
            
            if (i == *p_highlight)
            {
                wattron(ptr, A_REVERSE);
            }
            
            char buf[256] = "/";
            if (getfile.namelist[i]->d_type != 4)
            {
                buf[1] = '.';
            }

            strncat(buf, getfile.namelist[i]->d_name, WINDOW_WIDTH - 4);
            mvwprintw(ptr, i + 1, 1, buf);
            wattroff(ptr, A_REVERSE);
        }

        wrefresh(ptr);
        
        int val = wgetch(ptr);
        switch(val)
        {
            case KEY_UP:
            {   
                (*p_highlight)--;
                if (*p_highlight < 0)
                {
                    *p_highlight = 0;
                }
                break;
            }
            case KEY_DOWN:
            {   
                (*p_highlight)++;
                if (*p_highlight >= getfile.size)
                {
                    *p_highlight = getfile.size - 1;
                }
                break;
            }
            case 10:
            {
                if (getfile.namelist[*p_highlight]->d_type != 4)
                {
                    break;
                }

                if (0 == strcmp(getfile.namelist[*p_highlight]->d_name, "."))
                {
                    break;
                }
                
                if (0 == strcmp(getfile.namelist[*p_highlight]->d_name, ".."))
                {
                    int len = strlen(p_pwd) - 1;
                    for (int i = len; i >= 0; i--)
                    {
                        if (p_pwd[i] == '/')
                        {
                            p_pwd[i] = '\0';
                            break;
                        }
                    }
                    if (strlen(p_pwd) == 0)
                    {
                        p_pwd[0] = '/';
                        p_pwd[1] = 0;
                    }

                    chdir(p_pwd);
                    free_files(ptr, &getfile);
                    get_files(&getfile, p_pwd);
                    *p_highlight = 0;
                    break;
                }
                
                getcwd(p_pwd, 1024);
                strcat(p_pwd, "/");
                strncat(p_pwd, getfile.namelist[*p_highlight]->d_name, 1024);
                chdir(p_pwd);
                
                free_files(ptr, &getfile);
                get_files(&getfile, p_pwd);
                *p_highlight = 0;
                break;
            }

            case 9:
            {
                free_files(ptr, &getfile);
                
                if (ptr == first_win)
                {
                    ptr = sec_win;
                    p_pwd = pwd2;
                    chdir(p_pwd);
                    getcwd(p_pwd, 1024);
                    p_highlight = &highlight2;
                    
                }
                else if (ptr == sec_win)
                {
                    ptr = first_win;
                    p_pwd = pwd1;
                    chdir(p_pwd);
                    getcwd(p_pwd, 1024);
                    p_highlight = &highlight1;
                } 

                get_files(&getfile, p_pwd);
            }
        }
    }
    endwin();
    return 0;
}
