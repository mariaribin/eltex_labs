#define MESSAGESIZE 32
#define INPUT_MESSAGE_QUEUE "/new_queue"
#define OUTPUT_MESSAGE_QUEUE "/service_queue"
#define MESSAGENUMBER 10
#define MYQUEUE "/queue_%d"
#define PRIO 0

#include <stdbool.h>

struct Message
{
    int id;
    char text[MESSAGESIZE];
    char name[16];
    char queue_name[32];
    bool can_i_join; 
};