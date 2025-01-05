/*Написать 2 программы, первая сервер, вторая клиент. Сервер создает
очередь сообщений для реализации чата с общей комнатой (если
необходимо, то можно и больше очередей создать) и его задача
уведомлять клиентов о появлении новых участников, о новых
сообщениях. Клиент подключается к очереди, созданной сервером,
сообщает ему свое имя и получает в ответ все сообщения в комнате.
Далее может отправлять сообщения в общий чат. Получение служебных
сообщений от сервера (имена новых клиентов, сообщения от других
пользователей) и отправка сообщений в чат лучше реализовать в разных
потоках. Интерфейс клиента реализуем с помощью библиотеки ncurses.

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
#include <pthread.h>
#include <dirent.h>
#include "history.h"

mqd_t qd_sent = 0;
mqd_t qd_receive = 0;

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

void clean(char queue[])
{
    int ret = 0;
    
    ret = mq_close(qd_sent);
    if (-1 == ret)
    {
        perror("\nClosing queue error");
    }
 
    ret = mq_unlink(INPUT_MESSAGE_QUEUE);
    if (-1 == ret)
    {
        perror("\nQueue already removed");
    }

    ret = mq_close(qd_receive);
    if (-1 == ret)
    {
        perror("\nClosing queue error");
    }

    ret = mq_unlink(queue);
    if (-1 == ret)
    {
        perror("\nQueue already removed");
    }

    printf("\ncleaned queue!\n");

    exit(0);
}

void* send(void *arg)
{
    int ret = 0;
    char buf[20] = {0};
    
    struct Message *args = (struct Message *)arg;
    if (!args)
    {
        return NULL;
    }
    
    while (1)
    {
        mvwscanw(args->p_win2, 2, 18, "%s", buf);
        wrefresh(args->p_win2);
        strcpy(args->text, buf);
        ret = mq_send(qd_sent, (const char *)args, 
                      sizeof(struct Message), PRIO);
        if (-1 == ret)
        {
            perror("mq_send() failed");
        }
        mvwprintw(args->p_win2, 2, 18, "                   ");
        wrefresh(args->p_win2);    
    }
}

int main()
{
    /*struct sigaction sig = {0};
    sig.sa_handler = clean;
    int ret = sigaction(SIGINT, &sig, NULL);
    if (-1 == ret)
    {
        perror("\nsigaction() failed");
    }*/

    struct Message tmp = {0};
    struct Message send_message = {0};
    send_message.id = getpid();
    send_message.can_i_join = true;
    snprintf(send_message.queue_name, sizeof(send_message.queue_name), MYQUEUE, getpid());

    initscr();
    cbreak();
    curs_set(0);

    int yMax = 0;
    int xMax = 0;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW *first_win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 
                               WINDOW_TOP_BORDER, WINDOW_SIDE_BORDER);
    box(first_win, 0, 0);

    WINDOW *sec_win = newwin(WINDOW_2_HEIGHT, WINDOW_2_WIDTH, 
                             WINDOW_2_TOP_BORDER, WINDOW_2_SIDE_BORDER);
    box(sec_win, 0, 0);

    send_message.p_win1 = first_win;
    send_message.p_win2 = sec_win;

    refresh();
    wrefresh(first_win);
    wrefresh(sec_win);

    keypad(first_win, true);
    keypad(sec_win, true);

    mvwprintw(sec_win, 2, 2, "Enter name: ");
    wrefresh(sec_win);

    char buf[10] = {0}; 
    mvwscanw(sec_win, 2, 18, "%s", buf);
    strcpy(send_message.name, buf);
    wrefresh(sec_win);

    char joined[30] = {0};
    snprintf(joined, sizeof(joined), "[%s joined!]", send_message.name);

    unsigned int prio1 = 0;

    struct mq_attr attr_sent = {0};
    attr_sent.mq_msgsize = sizeof(struct Message);;
    attr_sent.mq_maxmsg = MESSAGENUMBER;

    qd_sent = mq_open(INPUT_MESSAGE_QUEUE, O_CREAT | O_RDWR, 0600, &attr_sent);
    if (-1 == qd_sent)
    {
        perror("\nmq_open() failed");
        clean(send_message.queue_name);
        return -1;
    }

    struct mq_attr attr_receive = {0};
    attr_receive.mq_msgsize = sizeof(struct Message);;
    attr_receive.mq_maxmsg = MESSAGENUMBER;

    qd_receive = mq_open(send_message.queue_name, O_CREAT | O_RDWR, 0600, &attr_receive);
    if (-1 == qd_sent)
    {
        perror("\nmq_open() failed");
        clean(send_message.queue_name);
        return -1;
    }
  
    mq_send(qd_sent, (const char *)&send_message, sizeof(struct Message), PRIO);

    int i = 1;

    for (i; i < 32; )
    {
        mq_receive(qd_receive, (char *)&tmp, sizeof(struct Message), &prio1); 
        if (0 == strlen(tmp.text))
        {
            send_message.can_i_join = false;
            wrefresh(first_win);
            break;
        }
        else
        {
            mvwprintw(first_win, i + 1, 2, "%s: %s", tmp.name, tmp.text);   
        }
        send_message.can_i_join = false;
        wrefresh(first_win);
        i++;
    }

    mvwprintw(sec_win, 2, 18, "                   ");
    wrefresh(sec_win);
    mvwprintw(sec_win, 2, 2, "Enter message: ");
    wrefresh(sec_win);

    pthread_t thread = 0;
    pthread_create(&thread, NULL, send, &send_message);
    pthread_detach(thread);

    while(1)
    {
        mq_receive(qd_receive, (char *)&tmp, sizeof(struct Message), &prio1);
        if (0 == strlen(tmp.text))
        {
            mvwprintw(first_win, 1, 4, "%s", "No messages in this chat yet.");
            mvwprintw(first_win, 2, 15, "%s", "Be first!");
            wrefresh(first_win);
            continue;
        }  
        if (2 == prio1)
        {
            mvwprintw(first_win, i + 3, 15, "%s", tmp.text);
        }
        else
        {
            mvwprintw(first_win, i + 3, 2, "%s: %s", tmp.name, tmp.text);
        }
       
        wrefresh(first_win);
        i++;
    }

    return 0;
}