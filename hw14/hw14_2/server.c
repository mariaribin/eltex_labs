/*Переписать чат-комнату с разделяемой памятью.

THIS IS THE SERVER*/

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

void clean()
{
    int ret = 0;

    // Remove storage shmem
    ret = munmap(g_storage, SHMEM_STORAGE_SIZE);
    if (-1 == ret)
    {
        perror("munmap() failed");
        return;
    }

    ret = close(g_shm_storage);
    if (-1 == ret)
    {
        perror("close() failed");
        return;
    }

    ret = shm_unlink(SHMEM_STORAGE);
    if (-1 == ret)
    {
        perror("shm_unlink() failed");
        return; 
    }

    // Remove reciever shmem
    ret = munmap(g_recieve, SHMEM_RECIEVE_SIZE);
    if (-1 == ret)
    {
        perror("munmap() failed");
        return;
    }

    ret = close(g_shm_recieve);
    if (-1 == ret)
    {
        perror("close() failed");
        return;
    }

    ret = shm_unlink(SHMEM_RECIEVE);
    if (-1 == ret)
    {
        perror("shm_unlink() failed");
        return; 
    }

    // Remove sems
    sem_close(g_storage_sem);
    sem_unlink(STORAGE_SEM);

    sem_close(g_reciever_sem);
    sem_unlink(RECIEVE_SEM);

    printf("\ncleaned queue!\n");
    exit(0);
}

int main()
{   
    int i = 0;
    int ret = 0;
    struct sigaction sig = {0};

    sig.sa_handler = clean;
    ret = sigaction(SIGINT, &sig, NULL);
    if (-1 == ret)
    {
        perror("\nsigaction() failed");
        return -1;
    }

    // Create storage and reciever sems
    g_storage_sem = sem_open(STORAGE_SEM, O_CREAT, 0600, 1);
    if (SEM_FAILED == g_storage_sem)
    {
        perror("sem_open()");
        clean();
        return -1;
    }

    g_reciever_sem = sem_open(RECIEVE_SEM, O_CREAT, 0600, 1);
    if (SEM_FAILED == g_reciever_sem)
    {
        perror("sem_open()");
        clean();
        return -1;
    }

    g_sender_sem = sem_open(SEND_SEM, O_CREAT, 0600, 1);
    if (SEM_FAILED == g_sender_sem)
    {
        perror("sem_open()");
        clean();
        return -1;
    }

    // Create storage shmem
    g_shm_storage = shm_open(SHMEM_STORAGE, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (-1 == g_shm_storage)
    {
        perror("storage shm_open() failed");
        return -1;
        clean();
        return -1;
    }

    ret = ftruncate(g_shm_storage, SHMEM_STORAGE_SIZE);
    if (-1 == ret)
    {
        perror("ftruncate() failed");
        clean();
        return -1;
    }

    g_storage = mmap(0, SHMEM_STORAGE_SIZE,  PROT_WRITE | PROT_READ, 
                     MAP_SHARED, g_shm_storage, 0);
    if (NULL == g_storage)
    {
        perror("mmap() failed");
        clean();
        return -1;
    }

    // Create reciever shmem
    g_shm_recieve = shm_open(SHMEM_RECIEVE, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (-1 == g_shm_recieve)
    {
        perror("receive shm_open() failed");
        clean();
        return -1;
    }

    ret = ftruncate(g_shm_recieve, SHMEM_RECIEVE_SIZE);
    if (-1 == ret)
    {
        perror("ftruncate() failed");
        clean();
        return -1;
    }

    g_recieve = mmap(0, SHMEM_RECIEVE_SIZE,  PROT_WRITE | PROT_READ, 
                     MAP_SHARED, g_shm_recieve, 0);
    if (NULL == g_recieve)
    {
        perror("mmap() failed");
        clean();
        return -1;
    }

    //Create sender shmem
    g_shm_send = shm_open(SHMEM_SEND, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (-1 == g_shm_send)
    {
        perror("send shm_open() failed");
        clean();
        return -1;
    }

    ret = ftruncate(g_shm_send, SHMEM_SEND_SIZE);
    if (-1 == ret)
    {
        perror("ftruncate() failed");
        clean();
        return -1;
    }

    g_send = mmap(0, SHMEM_SEND_SIZE, PROT_WRITE | PROT_READ, 
                  MAP_SHARED, g_shm_send, 0);
    if (NULL == g_send)
    {
        perror("mmap() failed");
        clean();
        return -1;
    }

    int storage_ind = 0;

    while(1)
    {    
        struct Message tmp = {0};
         
        //receiving message
        ret = sem_wait(g_reciever_sem);
        if (-1 == ret)
        {
            perror("sem_wait");
            clean();
            return -1;
        }

        if (!memcmp(&tmp, g_recieve, sizeof(struct Message)))
        {
            printf("Checking empty\n");
            ret = sem_post(g_reciever_sem);
            if (-1 == ret)
            {
                perror("sem_post");
                clean();
                return -1;
            }

            sleep(1);
            continue;
        }

        tmp = *g_recieve;

        printf("%s\n", tmp.text);

        memset(g_recieve, 0, sizeof(struct Message));

        ret = sem_post(g_reciever_sem);
        if (-1 == ret)
        {
            perror("sem_post");
            clean();
            return -1;
        }

        printf("Sending to storage\n");

        //sending to storage
        ret = sem_wait(g_storage_sem);
        if (-1 == ret)
        {
            perror("sem_wait");
            clean();
            return -1;
        }

        g_storage[storage_ind] = tmp;

        ret = sem_post(g_storage_sem);
        if (-1 == ret)
        {
            perror("sem_post");
            clean();
            return -1;
        } 

        //sending to everyone
        ret = sem_wait(g_sender_sem);
        if (-1 == ret)
        {
            perror("sem_wait");
            clean();
            return -1;
        }

        tmp.counter = storage_ind++;

        *g_send = tmp;

        printf("Sending to everyone: %s\n", g_send->text);

        usleep(100);

        ret = sem_post(g_sender_sem);
        if (-1 == ret)
        {
            perror("sem_post");
            clean();
            return -1;
        }
    }

    clean(); 
    return 0;
}