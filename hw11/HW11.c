/*Программа при запуске формирует массив из 5 интовых чисел 
и рандомно заполняет каждый элемент массива значением от 1 до 10 000. 

Каждое число — это "магазин". Главная программа порождает 3 потока покупателей.  
Покупатель при порождении имеет потребность — рандомное число в пределах 100 000. 

Покупатель пытается зайти в свободный магазин. 
Когда покупатель находит свободный магазин, он забирает оттуда все товары 
и уменьшает свою потребность на это количество товаров. Засыпает на 2 секунды, 
просыпается и опять ищет свободный магазин.

Когда покупатель достигает 0 своей потребности — поток завершает работу.

Помимо 3х потоков-покупателей есть еще 4ый поток — погрузчик. Заходя в свободны магазин, 
он добавляет туда 5 000 продуктов. Засыпает на 1 сек., просыпается, снова ищет.

Когда все покупатели достигли 0, основная программа завершает поток-погрузчик и завершается.*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <unistd.h>
#define THREADS 4
#define SHOPS 5
#define NEED 3

struct Data
{ 
    int *ptr_shop;
    int *ptr_need;
    int *plock;
    int *status;
    int *counter;
};

static struct Data g_data[THREADS];
static pthread_mutex_t g_mutex;

void *buy(void *args)
{
    struct Data *data = (struct Data*)args;
    if (!data)
    {
        return NULL;
    }

    while (*data->ptr_need > 0)
    {
        for (int i = 0; i < SHOPS; i++)
        {
            if (1 == *data->plock)
            {
                continue;
            }

            if(0 == *data->ptr_need)
            {
                break;
            }

            pthread_mutex_lock(&g_mutex);
            *data->plock = 1;

            if(*data->ptr_need < *(data->ptr_shop + i))
            {
                *data->ptr_need = 0;
                data->ptr_shop[i] -= *data->ptr_need;
            }
            else
            {
                *data->ptr_need -= data->ptr_shop[i];
                data->ptr_shop[i] = 0;
            }
            
            pthread_mutex_unlock(&g_mutex);
            *data->plock = 0;  

            if(0 == *data->ptr_need)
            {
                break;
            }

            sleep(2);
        }
    }
}

void *restock(void *args)
{
    struct Data *data = (struct Data*)args;

    while(1)
    {
        if (data->ptr_need[0] == 0 &&
            data->ptr_need[1] == 0 &&
            data->ptr_need[2] == 0)
        {
            break;
        }

        for (int i = 0; i < SHOPS; i++)
        {
            if (1 == *data->plock)
            {
                continue;
            }

            pthread_mutex_lock(&g_mutex);
            *data->plock = 1;

            data->ptr_shop[i] += 5000;

            pthread_mutex_unlock(&g_mutex);
            *data->plock = 0;

            sleep(1);
        }

        //PROGRESS INFO
        printf("\n\nNeed1 = %d, Need2 = %d, Need3 = %d\n", 
        data->ptr_need[0], data->ptr_need[1], data->ptr_need[2]);

        for (int i = 0; i < THREADS; i++)
        {
            printf("Thread status %d = %d\n", 
            i + 1, data->status[i]);
        }
    }
}

int main()
{
    pthread_t tid[THREADS] = {0};
    int status[THREADS] = {1, 1, 1, 1};
    int shop[SHOPS] = {0};
    int need[NEED] = {0};
    int lock = 0;

    for (int i = 0; i < (THREADS - 1); i++)
    {
        g_data[i].ptr_shop = shop;
        g_data[i].ptr_need = need + i;;
        g_data[i].plock = &lock;
        g_data[i].status = status;
    }

    g_data[THREADS - 1].ptr_shop = shop;
    g_data[THREADS - 1].ptr_need = need;
    g_data[THREADS - 1].plock = &lock;
    g_data[THREADS - 1].status = status;

    pthread_mutex_init(&g_mutex, NULL);

    srand(time(0));

    for (int i = 0; i < SHOPS; i++)
    {
        shop[i] = rand() % 10000;
    }

    for (int i = 0; i < NEED; i++)
    {
        need[i] = rand() % 100000;
    }

    for (int i = 0; i < SHOPS; i++)
    {
        printf("Goods in shop %d: %d\n", 
        i + 1, g_data[0].ptr_shop[i]);
    }

    for (int i = 0; i < NEED; i++)
    {
        printf("Thread %d needs: %d\n", 
        i + 1, *g_data[i].ptr_need);
    }

    printf("Please wait.............\n");

    int ret = pthread_create(&tid[THREADS - 1], NULL, restock, &g_data[THREADS - 1]);
    if (-1 == ret)
    {
        perror("pthread_create() failed");
        pthread_mutex_destroy(&g_mutex);
        return 0;
    }

    for (int i = 0; i < (THREADS - 1); i++)
    {
        ret = pthread_create(&tid[i], NULL, buy, &g_data[i]);
        if (-1 == ret)
        {
            perror("pthread_create() failed");
            pthread_mutex_destroy(&g_mutex);
            return 0;
        }
    }

    for (int i = 0; i < THREADS; i++)
    {
        status[i] = pthread_join(tid[i], NULL);
    }
    
    printf("\n\nNeed1 = %d, Need2 = %d, Need3 = %d\n", 
    need[0], need[1], need[2]);

    for (int i = 0; i < THREADS; i++)
    {
        printf("Thread status %d returned %d\n", i + 1, status[i]);
    }

    pthread_mutex_destroy(&g_mutex);
    return 0;
}
