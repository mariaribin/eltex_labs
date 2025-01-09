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


#define SHMEM_STORAGE "/server_storage"
#define SHMEM_RECIEVE "/server_recieve"
#define SHMEM_SEND "/server_send"
#define STORAGE_SEM "/storage_sem"
#define RECIEVE_SEM "/recieve_sem"
#define SEND_SEM "/send_sem"
#define SHMEM_STORAGE_SIZE 32 * sizeof(struct Message)
#define SHMEM_RECIEVE_SIZE sizeof(struct Message)
#define SHMEM_SEND_SIZE sizeof(struct Message)

int g_shm_storage = 0;
int g_shm_recieve = 0;
int g_shm_send = 0;
sem_t *g_storage_sem = NULL;
sem_t *g_reciever_sem = NULL;
sem_t *g_sender_sem = NULL;
struct Message *g_storage = NULL;
struct Message *g_recieve = NULL;
struct Message *g_send = NULL;

char name[8] = {0};

/*struct Arg
{
    struct Message *p_message;
};*/

void clean()
{
}

void* send(void *arg)
{
    struct Message message = {0};
    strncpy(message.name, name, sizeof(name));
    while (1)
    {
        

        printf("Enter message: ");

        scanf("%s", message.text);

        int ret = sem_wait(g_reciever_sem);
        if (-1 == ret)
        {
            perror("sem_wait");
            clean();
        }

        *g_recieve = message;

        ret = sem_post(g_reciever_sem);
        if (-1 == ret)
        {
            perror("sem_post");
            clean();
        }

        usleep(100);

    }
}

int main()
{   
    struct Message init = {0}; 
    int ret = 0;
    
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


    //Add storage shmem
    g_shm_storage = shm_open(SHMEM_STORAGE, O_EXCL | O_RDWR, 0600);
    if (-1 == g_shm_storage)
    {
        perror("shm_open() failed");
        return -1;
        clean();
        return -1;
    }

    g_storage = mmap(0, SHMEM_STORAGE_SIZE,  PROT_WRITE | PROT_READ, MAP_SHARED, g_shm_storage, 0);
    if (NULL == g_storage)
    {
        perror("mmap() failed");
        clean();
        return -1;
    }

    //Add server_reveive_from_client shmem
    g_shm_recieve = shm_open(SHMEM_RECIEVE, O_EXCL | O_RDWR, 0600);
    if (-1 == g_shm_recieve)
    {
        perror("shm_open() failed");
        clean();
        return -1;
    }

    g_recieve = mmap(0, SHMEM_RECIEVE_SIZE,  PROT_WRITE | PROT_READ, MAP_SHARED, g_shm_recieve, 0);
    if (NULL == g_recieve)
    {
        perror("mmap() failed");
        clean();
        return -1;
    }

    //Add server_send_to_client shmem
    g_shm_send = shm_open(SHMEM_SEND, O_EXCL | O_RDWR, 0600);
    if (-1 == g_shm_send)
    {
        perror("shm_open() failed");
        clean();
        return -1;
    }

    g_send = mmap(0, SHMEM_SEND_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, g_shm_send, 0);
    if (NULL == g_send)
    {
        perror("mmap() failed");
        clean();
        return -1;
    }


    printf("Enter name: ");
    scanf("%s", name);
    /////

    ret = sem_wait(g_storage_sem);
    if (-1 == ret)
    {
        perror("sem_wait");
        clean();
        return -1;
    }

    for (int i = 0; i < 32; i++)
    {
        printf("%s: %s\n", g_storage[i].name, g_storage[i].text);
       
        if (!memcmp(&init, &g_storage[i + 1], sizeof(struct Message)))
        {
            break;
        }
    }

    ret = sem_post(g_storage_sem);
    if (-1 == ret)
    {
        perror("sem_post");
        clean();
        return -1;
    }

    pthread_t thread = 0;
    pthread_create(&thread, NULL, send, NULL);
    pthread_detach(thread);

    struct Message a = {0};
    
    while (1)

    { 
        struct Message init = {0};
        
        /*ret = sem_wait(g_sender_sem);
        if (-1 == ret)
        {
            perror("sem_wait");
            clean();
            return -1;
        }*/

        if (0 == memcmp(&init, g_send, sizeof(struct Message)))
        {   
            /*ret = sem_post(g_sender_sem);
            if (-1 == ret)
            {
                perror("sem_post");
                clean();
                return -1;
            }*/

            usleep(100);
            continue;
        }

        a = *g_send;   

        printf("%s: %s", a.name, a.text);


    

        sleep(1);

        /*ret = sem_post(g_sender_sem);
        if (-1 == ret)
        {
            perror("sem_post");
            clean();
            return -1;
        }*/



    }
    
   
   /* struct sigaction sig = {0};
    sig.sa_handler = clean;
    int ret = sigaction(SIGINT, &sig, NULL);
    if (-1 == ret)
    {
        perror("\nsigaction() failed");
    }

    struct Message message = {0};
    message.id = getpid();
    char buf[10] = {0};

    printf("Enter name: ");
    scanf("%s", buf);
    strcpy(message.name, buf);

    message.can_i_join = true;
    snprintf(message.queue_name, sizeof(message.queue_name), MYQUEUE, getpid());

    struct Message tmp = {0};

    int prio1 = 0;

    struct mq_attr attr_sent = {0};
    attr_sent.mq_msgsize = sizeof(struct Message);;
    attr_sent.mq_maxmsg = MESSAGENUMBER;

    qd_sent = mq_open(INPUT_MESSAGE_QUEUE, O_CREAT | O_RDWR, 0600, &attr_sent);
    if (-1 == qd_sent)
    {
        perror("\nCreating messages queue failed");
        clean();
        return -1;
    }

    struct mq_attr attr_receive = {0};
    attr_receive.mq_msgsize = sizeof(struct Message);;
    attr_receive.mq_maxmsg = MESSAGENUMBER;

    qd_receive = mq_open(message.queue_name, O_CREAT | O_RDWR, 0600, &attr_receive);
    if (-1 == qd_sent)
    {
        perror("\nCreating messages queue failed");
        clean();
        return -1;
    }
  
    mq_send(qd_sent, (const char *)&message, sizeof(struct Message), PRIO);

    for (int i = 0; i < 32; i++)
    {
        mq_receive(qd_receive, (char *)&tmp, sizeof(struct Message), &prio1);
        printf("%s: %s\n", tmp.name, tmp.text);
        if (true == message.can_i_join)
        {
            message.can_i_join = false;
        }
        if (0 == strlen(tmp.text))
        {
            break;
        }
    }

    struct Arg arg = {0};
    arg.p_message = &message;

    pthread_t thread = 0;
    pthread_create(&thread, NULL, send, &arg);
    pthread_detach(thread);

    while(1)
    {
        mq_receive(qd_receive, (char *)&tmp, sizeof(struct Message), &prio1);

        if (2 == prio1)
        {
            printf("%s\n", tmp.text);
        }
        else
        {
            printf("%s: %s\n", tmp.name, tmp.text);
        }
        //if (0 != strcmp(tmp.queue_name, message.queue_name))
       // {
            //printf("%s: %s\n", tmp.name, tmp.text);
       // } 
        
    }

    clean();

    */
    return 0;
}