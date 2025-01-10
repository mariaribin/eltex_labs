/*Реализовать 2 программы, первая сервер, вторая клиент. Сервер создает
область разделяемой памяти и записывает сообщение виде строки “Hi!”, ждет
ответа от клиента и выводит на экран, удаляет очередь. Клиент
присоединяет выделенную область, считывает сообщение от сервера, выводит на
экран, отвечает серверу сообщением виде строки “Hello!”. Сделать это
как для POSIX, так и для SYSTEM V стандартов.

THIS IS THE CLIENT*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define SEMNAME "/dev/shm/semaphore"
#define SHARED_MEMORY_OBJECT_NAME "/dev/shm/shmemory"
#define SHARED_MEMORY_OBJECT_SIZE 128

int main ()
{
    struct sembuf sem[2] = {0};
    key_t key = 0;
    key_t semkey = 0;
    int semid = 0;
    int semfd = 0;
    int shmid = 0;
    int fd = 0;
    int ret = 0;
    char message[] = "Hello!";
    char init[10] = {0};
    char *shm_addr = NULL;

    sem[0].sem_flg = 0;
    sem[0].sem_num = 0;
    sem[0].sem_op = -1;
    sem[1].sem_flg = SEM_UNDO;
    sem[1].sem_num = 0;
    sem[1].sem_op = 1;

    //Adding shared memory
    key = ftok(SHARED_MEMORY_OBJECT_NAME, 777);

    shmid = shmget(key, SHARED_MEMORY_OBJECT_SIZE | O_EXCL, 0600);
    if (-1 == shmid)
    {
        perror("shmget() failed");
        return -1;
    }

    shm_addr = shmat(shmid, 0, 0);
    if (NULL == shm_addr)
    {
        perror("shmat() failed");
        return -1;
    }

    //Adding semaphore
    semkey = ftok(SEMNAME, 888);
    if (-1 == semkey)
    {
        perror("ftok() failed");
        return -1;
    }

    semid = semget(semkey, 3, 0600);
    if (-1 == semid)
    {
        perror("semget() failed");
        return -1;
    }


    ret = semop(semid, &sem[0], 1);
    if (-1 == ret)
    {
        perror("semop1() failed");
        return -1;
    }
    
    if (0 != strncmp(message, shm_addr, sizeof(message)))
    {
        printf("Server sent: %s\n", shm_addr);   
    } 

    ret = semop(semid, &sem[1], 1);
    if (-1 == ret)
    {
        perror("semop1() failed");
        return -1;
    }

    printf("Sending back!\n");
    //Writing to shared memory 
    memcpy(shm_addr, message, sizeof(message));
    shmdt(shm_addr);
    return 0;
}