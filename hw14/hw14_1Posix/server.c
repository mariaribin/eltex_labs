#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SEMNAME "/test_sem123"
#define SHARED_MEMORY_OBJECT_NAME "/test_shmemory"
#define SHARED_MEMORY_OBJECT_SIZE 128
#define TEST_STR "Hello!"

int main ()
{
    int shm = 0;
    int ret = 0;
    sem_t *sem;
    char *addr = NULL;
    
    //create sh_mem
    shm = shm_open(SHARED_MEMORY_OBJECT_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (-1 == shm)
    {
        perror("shm_open() failed");
        return -1;
    }

    ret = ftruncate(shm, SHARED_MEMORY_OBJECT_SIZE);
    if (-1 == ret)
    {
        perror("ftruncate() failed");
        return -1;
    }

    addr = mmap(0, SHARED_MEMORY_OBJECT_SIZE,  PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
    if (NULL == addr)
    {
        perror("mmap() failed");
        return -1;
    }

    //create semaphore
    sem = sem_open(SEMNAME, O_CREAT, 0600, 1);
    if (SEM_FAILED == sem)
    {
        perror("sem_opne()");
        return -1;
    }

    ret = sem_wait(sem);
    if (-1 == ret)
    {
        perror("sem_wait");
        return -1;
    }

    // set data
    memcpy(addr, TEST_STR, sizeof(TEST_STR));

    ret = sem_post(sem);
    if (-1 == ret)
    {
        perror("sem_post");
        return -1;
    }

    while (1)
    {
        sleep(1);

        if (0 != strncmp(TEST_STR, addr, sizeof(TEST_STR)))
        {
            printf("Client sent: %s\n", addr);
            break;
        }
    }

    ret = munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
    if (-1 == ret)
    {
        perror("munmap() failed");
        return -1;
    }

    ret = close(shm);
    if (-1 == ret)
    {
        perror("close() failed");
        return -1;
    }

    ret = shm_unlink(SHARED_MEMORY_OBJECT_NAME);
    if (-1 == ret)
    {
        perror("shm_unlink() failed");
        return -1; 
    }

    sem_close(sem);
    sem_unlink(SEMNAME);
    return 0;
}
