#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#define BUFSIZE 32
#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define TIMEOUT 5

int receive_and_answer_TCP(int client_socket)
{
    int ret = 0;
    char server_message[10] = {"Hello!"};
    char message[BUFSIZE] = {0};
 
    ret = recv(client_socket, &message, sizeof(message), 0);
    if (-1 == ret)
    {
        perror("Reception from tcp client failed.");
        return -1;
    }

    printf("Client sent: %s\n", message);

    ret = send(client_socket, server_message, sizeof(server_message), 0);
    if (-1 == ret)
    {
        perror("Sending to tcp client failed");
        return -1;
    }

    return 0;
}

int receive_and_answer_UDP(int server_socket)
{
    int ret = 0;
    char server_message[10] = {"Hello!"};
    char message[BUFSIZE] = {0};

    struct sockaddr addr;
    socklen_t len = sizeof(addr);

    ret = recvfrom(server_socket, 
                   &message, 
                   sizeof(message), 
                   0, 
                   (struct sockaddr *restrict)&addr, 
                   &len);
    if (-1 == ret)
    {
        perror("Reception from udp client failed.");
        return -1;
    }

    printf("Client sent: %s\n", message);

    ret = sendto(server_socket, 
                 server_message, 
                 sizeof(server_message), 
                 0, 
                 (struct sockaddr *restrict)&addr, 
                 len);
    if (-1 == ret)
    {
        perror("Sending to udp client failed");
        return -1;
    }

    return 0;
}

int main()
{
    int ret = 0;
    int server_socket_tcp = 0;
    int server_socket_udp = 0;
    int client_socket_tcp = 0;

    server_socket_tcp = socket(AF_INET, SOCK_STREAM, 0);
    server_socket_udp = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in server_address_tcp;
    server_address_tcp.sin_family = AF_INET;
    server_address_tcp.sin_port = htons(5000);
    server_address_tcp.sin_addr.s_addr = INADDR_ANY;

    struct sockaddr_in server_address_udp;
    server_address_udp.sin_family = AF_INET;
    server_address_udp.sin_port = htons(5001);
    server_address_udp.sin_addr.s_addr = INADDR_ANY;

    ret = bind(server_socket_tcp, (const struct sockaddr*)&server_address_tcp, 
               sizeof(server_address_tcp));
    

    ret = bind(server_socket_udp, (const struct sockaddr*)&server_address_udp, 
               sizeof(server_address_udp));

    
    ret = listen(server_socket_tcp, 3);

    fd_set readfds;
    struct timeval tv;
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(server_socket_tcp, &readfds);
    FD_SET(server_socket_udp, &readfds);

    int nfds = MAX(server_socket_tcp, server_socket_udp);

    while(1)
    {   
        ret = select(nfds + 1, &readfds, NULL, NULL, NULL);
        if (-1 == ret)
        {
            perror("select() failed");
            continue;
        }
        else if (0 == ret)
        {
            sleep(1); 
            continue;
        }
        else if (FD_ISSET(server_socket_tcp, &readfds))
        {
            printf("tcp\n");
            
            client_socket_tcp = accept(server_socket_tcp, NULL, NULL);
            receive_and_answer_TCP(client_socket_tcp);
            close(client_socket_tcp);
        }
        else if (FD_ISSET(server_socket_udp, &readfds))
        {
            printf("udp\n");
            
            receive_and_answer_UDP(server_socket_udp);
        }
    }

    return 0;
}