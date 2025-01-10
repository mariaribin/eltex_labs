/*Реализовать 2 программы, первая сервер, вторая клиент. Сервер создает
область разделяемой памяти и записывает сообщение виде строки “Hi!”, ждет
ответа от клиента и выводит на экран, удаляет очередь. Клиент
присоединяет выделенную область, считывает сообщение от сервера, выводит на
экран, отвечает серверу сообщением виде строки “Hello!”. Сделать это
как для POSIX, так и для SYSTEM V стандартов.

THIS IS THE SERVER*/

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
    int shmid = 0;
    int semid = 0;
    int fd = 0;
    int semfd = 0;
    int  ret = 0;
    char message[] = "Hi!"; 
    char *shm_addr = NULL;

    sem[0].sem_flg = IPC_NOWAIT;
    sem[0].sem_num = 0;
    sem[0].sem_op = 0;
    sem[1].sem_flg = SEM_UNDO;
    sem[1].sem_num = 0;
    sem[1].sem_op = 1;

    //Creating shared memory
    fd = open(SHARED_MEMORY_OBJECT_NAME, O_CREAT | O_RDWR | 0600);
    if (-1 == fd)
    {
        perror("open() failed");
        return -1;
    }

    key = ftok(SHARED_MEMORY_OBJECT_NAME, 777);
    if (-1 == key)
    {
        perror("ftok() failed");
        return -1;
    }

    shmid = shmget(key, SHARED_MEMORY_OBJECT_SIZE, IPC_CREAT | 0600);
    if (-1 == shmid)
    {
        perror("shmget() failed");
        shmctl(shmid, IPC_RMID, NULL);
        return -1;
    }

    shm_addr = shmat(shmid, 0, 0);
    if (NULL == shm_addr)
    {
        perror("shmat() failed");
        return -1;
    }

    //Creating semaphore
    semfd = open(SEMNAME, O_CREAT | O_RDWR | 0600);
    if (-1 == fd)
    {
        perror("open() failed");
        return -1;
    }

    semkey = ftok(SEMNAME, 888);
    if (-1 == semkey)
    {
        perror("ftok() failed");
        return -1;
    }

    semid = semget(semkey, 3, IPC_CREAT | 0600);
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

    //Writing to shared memory 
    memcpy(shm_addr, message, sizeof(message));

    ret = semop(semid, &sem[1], 1);
    if (-1 == ret)
    {
        perror("semop2() failed");
        return -1;
    }

    while (1)
    {     
        if (0 != strncmp(message, shm_addr, sizeof(message)))
        {
            printf("Client sent: %s\n", shm_addr);
            break;
        }

        sleep(1);
    }

    semctl(semid, 0, IPC_RMID); 
    shmdt(shm_addr);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
