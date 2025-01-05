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

struct Arg
{
    struct Message *p_message;
};

/*void clean()
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

    printf("\ncleaned queue!\n");

    exit(0);
}*/

void* send(void *arg)
{
    char buf[20] = {0};
    
    struct Arg *args = (struct Arg *)arg;
    if (!args)
    {
        return NULL;
    }
    
    while (1)
    {
        mvwprintw(args->p_message->p_win2, 2, 18, "         ");
        wrefresh(args->p_message->p_win2);
        mvwscanw(args->p_message->p_win2, 2, 18, "%s", buf);
        wrefresh(args->p_message->p_win2);
        strcpy(args->p_message->text, buf);
        mq_send(qd_sent, (const char *)args->p_message, sizeof(struct Message), PRIO);
    }
}

int main()
{
   /* struct sigaction sig = {0};
    sig.sa_handler = clean;
    int ret = sigaction(SIGINT, &sig, NULL);
    if (-1 == ret)
    {
        perror("\nsigaction() failed");
    }*/

    struct Message message = {0};
    message.id = getpid();
    char buf[10] = {0};

    initscr();
    //noecho();
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

    message.p_win1 = first_win;
    message.p_win2 = sec_win;

    refresh();
    wrefresh(first_win);
    wrefresh(sec_win);

    keypad(first_win, true);
    keypad(sec_win, true);

    mvwprintw(sec_win, 2, 2, "Enter name: ");
    wrefresh(sec_win);

    char sym = 0;
    int i = 0;
    
    mvwscanw(sec_win, 2, 18 + i, "%s", buf);
    strcpy(message.name, buf);
    wrefresh(sec_win);

    char buf1[30] = {0};
    snprintf(buf1, sizeof(buf1), "%s joined!", message.name);

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
        //clean();
        return -1;
    }

    struct mq_attr attr_receive = {0};
    attr_receive.mq_msgsize = sizeof(struct Message);;
    attr_receive.mq_maxmsg = MESSAGENUMBER;

    qd_receive = mq_open(message.queue_name, O_CREAT | O_RDWR, 0600, &attr_receive);
    if (-1 == qd_sent)
    {
        perror("\nCreating messages queue failed");
        //clean();
        return -1;
    }
  
    mq_send(qd_sent, (const char *)&message, sizeof(struct Message), PRIO);

    for (i = 0; i < 32; i++)
    {
        mq_receive(qd_receive, (char *)&tmp, sizeof(struct Message), &prio1);
        if (0 == strcmp(tmp.text, buf1))
        {
            continue;
        } 
        if (0 == strlen(tmp.text))
        {
            break;
        }
        
        mvwprintw(first_win, 2 + i, 2, "%s: %s", tmp.name, tmp.text);
        wrefresh(first_win);

        if (true == message.can_i_join)
        {
            message.can_i_join = false;
        }
    }

    mvwprintw(sec_win, 2, 18, "         ");
    wrefresh(sec_win);
    mvwprintw(sec_win, 2, 2, "Enter message: ");
    wrefresh(sec_win);

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
            mvwprintw(first_win, 2 + i, 2, "%s", tmp.text);
        }
        else
        {
            mvwprintw(first_win, 2 + i, 2, "%s: %s", tmp.name, tmp.text);
        }
       
        wrefresh(first_win);
        i++;
    }

    return 0;
}