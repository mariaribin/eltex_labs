#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#define SHARED_MEMORY_OBJECT_NAME "/test_shmemory"
#define SHARED_MEMORY_OBJECT_SIZE 128

#define TEST_STR "HI!!!!"

int main ()
{
    int shm = 0;
    int ret = 0;
    char *addr = NULL;
    
    shm = shm_open(SHARED_MEMORY_OBJECT_NAME, O_EXCL | O_RDWR, 0600);
    if (-1 == shm)
    {
        perror("shm_open() failed");
        return -1;
    }

    addr = mmap(0, SHARED_MEMORY_OBJECT_SIZE,  PROT_WRITE | PROT_READ, MAP_SHARED, shm, 0);
    if (NULL == addr)
    {
        perror("mmap() failed");
        return -1;
    }

    printf("Data: %s\n", addr);
    memcpy(addr, TEST_STR, sizeof(TEST_STR));

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

    return 0;
}