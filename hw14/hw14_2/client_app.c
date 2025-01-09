/*Переписать чат-комнату с разделяемой памятью.

THIS IS THE CLIENT*/

#include <fcntl.h>  
#include <mqueue.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>
#include "history.h"

int g_shm_storage = 0;
int g_shm_recieve = 0;
int g_shm_send = 0;
sem_t *g_storage_sem = NULL;
sem_t *g_reciever_sem = NULL;
sem_t *g_sender_sem = NULL;
struct Message *g_storage = NULL;
struct Message *g_recieve = NULL;
struct Message *g_send = NULL;
WINDOW *first_win = NULL;
WINDOW *sec_win = NULL;
char name[8] = {0};

enum Window_borders
{
    WINDOW_HEIGHT = 20,
    WINDOW_WIDTH = 40,
    WINDOW_TOP_BORDER = 0,
    WINDOW_SIDE_BORDER = 1,

    WINDOW_2_HEIGHT = 5,
    WINDOW_2_WIDTH = 40,
    WINDOW_2_TOP_BORDER = WINDOW_HEIGHT,
    WINDOW_2_SIDE_BORDER = 1,
};

void* send(void *arg)
{
    struct Message message = {0};


    strncpy(message.name, name, sizeof(name));
    while (1)
    {
        //mvwscanw(sec_win, 2, 18, "%s", message.text);
        mvwgetstr(sec_win, 2, 18, message.text);

        int ret = sem_wait(g_reciever_sem);
        if (-1 == ret)
        {
            perror("sem_wait");
        }

        *g_recieve = message;

        ret = sem_post(g_reciever_sem);
        if (-1 == ret)
        {
            perror("sem_post");
        }

        mvwprintw(sec_win, 2, 18, "                   ");
        wrefresh(sec_win);
        usleep(100);
    }
}

int main()
{
    int ret = 0;
    struct Message init = {0};  
    
    // Create storage and reciever sems
    g_storage_sem = sem_open(STORAGE_SEM, O_CREAT, 0600, 1);
    if (SEM_FAILED == g_storage_sem)
    {
        perror("sem_open()");
        return -1;
    }

    g_reciever_sem = sem_open(RECIEVE_SEM, O_CREAT, 0600, 1);
    if (SEM_FAILED == g_reciever_sem)
    {
        perror("sem_open()");
        return -1;
    }

    g_sender_sem = sem_open(SEND_SEM, O_CREAT, 0600, 3);
    if (SEM_FAILED == g_sender_sem)
    {
        perror("sem_open()");
        return -1;
    }


    //Add storage shmem
    g_shm_storage = shm_open(SHMEM_STORAGE, O_EXCL | O_RDWR, 0600);
    if (-1 == g_shm_storage)
    {
        perror("shm_open() failed");
        return -1;
    }

    g_storage = mmap(0, SHMEM_STORAGE_SIZE,  PROT_WRITE | PROT_READ, 
                        MAP_SHARED, g_shm_storage, 0);
    if (NULL == g_storage)
    {
        perror("mmap() failed");
        return -1;
    }

    //Add server_reveive_from_client shmem
    g_shm_recieve = shm_open(SHMEM_RECIEVE, O_EXCL | O_RDWR, 0600);
    if (-1 == g_shm_recieve)
    {
        perror("shm_open() failed");
        return -1;
    }

    g_recieve = mmap(0, SHMEM_RECIEVE_SIZE,  PROT_WRITE | PROT_READ, 
                     MAP_SHARED, g_shm_recieve, 0);
    if (NULL == g_recieve)
    {
        perror("mmap() failed");
        return -1;
    }

    //Add server_send_to_client shmem
    g_shm_send = shm_open(SHMEM_SEND, O_EXCL | O_RDWR, 0600);
    if (-1 == g_shm_send)
    {
        perror("shm_open() failed");
        return -1;
    }

    g_send = mmap(0, SHMEM_SEND_SIZE, PROT_WRITE | PROT_READ, 
                  MAP_SHARED, g_shm_send, 0);
    if (NULL == g_send)
    {
        perror("mmap() failed");
        return -1;
    }

    /*Here starts the NCURSES shell*/

    initscr();
    cbreak();
    curs_set(0);

    int yMax = 0;
    int xMax = 0;
    getmaxyx(stdscr, yMax, xMax);

    first_win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 
                               WINDOW_TOP_BORDER, WINDOW_SIDE_BORDER);
    box(first_win, 0, 0);

    sec_win = newwin(WINDOW_2_HEIGHT, WINDOW_2_WIDTH, 
                             WINDOW_2_TOP_BORDER, WINDOW_2_SIDE_BORDER);
    box(sec_win, 0, 0);

    refresh();
    wrefresh(first_win);
    wrefresh(sec_win);

    keypad(first_win, true);
    keypad(sec_win, true);

    mvwprintw(sec_win, 2, 2, "Enter name: ");
    wrefresh(sec_win);

    mvwscanw(sec_win, 2, 18, "%s", name);
    wrefresh(sec_win);

    char joined[30] = {0};
    snprintf(joined, sizeof(joined), "[%s joined!]", name);

    mvwprintw(sec_win, 2, 18, "                   ");
    wrefresh(sec_win);
    mvwprintw(sec_win, 2, 2, "Enter message: ");
    wrefresh(sec_win);

    ret = sem_wait(g_storage_sem);
    if (-1 == ret)
    {
        perror("sem_wait");
        return -1;
    }

    int i = 0;
    for (i; i < 32; i++)
    {
        if (0 == memcmp(&init, g_storage, sizeof(struct Message)))
        {
            mvwprintw(first_win, 1, 4, "%s", "No messages in this chat yet.");
            mvwprintw(first_win, 2, 15, "%s", "Be first!");
            i = i + 3;
            wrefresh(first_win);
            break;
        }

        mvwprintw(first_win, i + 1, 2, "%s: %s", g_storage[i].name, g_storage[i].text);
        wrefresh(first_win);

        if (!memcmp(&init, &g_storage[i + 1], sizeof(struct Message)))
        {
            break;
        }
    }

    ret = sem_post(g_storage_sem);
    if (-1 == ret)
    {
        perror("sem_post");
        return -1;
    }

    pthread_t thread = 0;
    pthread_create(&thread, NULL, send, NULL);
    pthread_detach(thread);

    int count = 1;

    while(1)
    {
        if (0 == memcmp(&init, g_send, sizeof(struct Message)))
        {   
            usleep(100);
            continue;
        }
        if (g_send->counter == count)
        {
            usleep(100);
            continue;
        }

        ret = sem_wait(g_sender_sem);
        if (-1 == ret)
        {
            perror("sem_wait");
            return -1;
        }

        count = g_send->counter;
        mvwprintw(first_win, i + 2, 2, "%s: %s", g_send->name, g_send->text);
        wrefresh(first_win);
        i++;
        usleep(100);

        ret = sem_post(g_sender_sem);
        if (-1 == ret)
        {
            perror("sem_post");
            return -1;
        }
    }

    return 0;
}