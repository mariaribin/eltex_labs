#ifndef HISTORY_H_NAME
#define HISTORY_H_NAME

#define SHMEM_STORAGE "/server_storage"
#define SHMEM_RECIEVE "/server_recieve"
#define SHMEM_SEND "/server_send"
#define STORAGE_SEM "/storage_sem"
#define RECIEVE_SEM "/recieve_sem"
#define SEND_SEM "/send_sem"
#define SHMEM_STORAGE_SIZE 32 * sizeof(struct Message)
#define SHMEM_RECIEVE_SIZE sizeof(struct Message)
#define SHMEM_SEND_SIZE sizeof(struct Message)
#include <ncurses.h>

struct Message
{
    int counter;
    char text[32];
    char name[8];
    WINDOW *p_win1;
    WINDOW *p_win2;
};


#endif



