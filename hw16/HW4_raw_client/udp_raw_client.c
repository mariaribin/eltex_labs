/*Написать RAW-сокет. UDP-сервер ждет строку от клиента на RAW-сокете 
и отвечает приветственным сообщением. */


#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define ETHER_TYPE  0x0800
#define SRC_IP_ADDR "192.168.0.107"
#define DST_IP_ADDR "192.168.0.114"
#define SRC_PORT 5000
#define DST_PORT 5000
#define IP_OFFSET 20
#define UDP_OFFSET 8
#define ETH_OFFSET 14
#define OFFSET (IP_OFFSET + UDP_OFFSET + ETH_OFFSET)

unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int main()
{
  int sockfd = 0;
  int ret = 0;
  const char ifname[] = "eno1";
  struct ifreq if_idx = {0};
  struct ifreq if_mac = {0};

  /* Open RAW socket to send on */
  sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETHER_TYPE));
  if (-1 == sockfd)
  {
      perror("socket() failed");
      return -1;
  }

  // Get the index of the interface 
  strcpy(if_idx.ifr_name, ifname);

  ret = ioctl(sockfd, SIOCGIFINDEX, &if_idx);
  if (-1 == ret)
  {
      perror("ioctl() for SIOCGIFINDEX failed");
      return -1;
  }

  char message[128] = {0};
  struct ether_header *eth_hdr = (struct ether_header *)message;
  struct iphdr *ip_hdr = (struct iphdr *) (message + sizeof(struct ether_header));
  struct udphdr *udp_hdr = (struct udphdr *) (message + sizeof(struct ether_header) + sizeof(struct iphdr));

  /* Construct Ethernet header */
  for (int i = 0; i < ETH_ALEN; i++)
  {
    eth_hdr->ether_shost[i] = 0;
  }

  eth_hdr->ether_shost[0] = 0x18;
  eth_hdr->ether_shost[1] = 0xc0;
  eth_hdr->ether_shost[2] = 0x4d;
  eth_hdr->ether_shost[3] = 0xee;
  eth_hdr->ether_shost[4] = 0xf2;
  eth_hdr->ether_shost[5] = 0x87;

  eth_hdr->ether_dhost[0] = 0x14;
  eth_hdr->ether_dhost[1] = 0x13;
  eth_hdr->ether_dhost[2] = 0x33;
  eth_hdr->ether_dhost[3] = 0x38;
  eth_hdr->ether_dhost[4] = 0x77;
  eth_hdr->ether_dhost[5] = 0x1d;

  eth_hdr->ether_type = htons(ETH_P_IP);

  /* Construct IP header */
  ip_hdr->ihl = 5;
  ip_hdr->ttl = 255;
  ip_hdr->version = IPVERSION;
  ip_hdr->protocol = IPPROTO_UDP;
  inet_pton(AF_INET, SRC_IP_ADDR, &ip_hdr->saddr);
  inet_pton(AF_INET, DST_IP_ADDR, &ip_hdr->daddr);
  ip_hdr->tot_len = htons(sizeof(message) - sizeof(struct ether_header));

  ip_hdr->check = 0;
  ip_hdr->check = checksum(ip_hdr, sizeof(struct iphdr));
  //ip_hdr->check = checksum(ip_hdr);

  /* Construct UDP header */
  udp_hdr->source = htons(SRC_PORT);
  udp_hdr->dest = htons(DST_PORT);
  udp_hdr->len = htons(sizeof(message) - sizeof(struct ether_header) - sizeof(struct iphdr));
  udp_hdr->check = 0;

  /* Prepare sockaddr_ll struct */
  struct sockaddr_ll sock_addr = {0};
  sock_addr.sll_ifindex = if_idx.ifr_ifindex;
  sock_addr.sll_halen = ETH_ALEN;
  sock_addr.sll_family = AF_PACKET;

  int offset = sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr);

  strcpy(message + offset, "Hi!");

  ret = sendto(sockfd, message, sizeof(message), 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr));
  if (-1 == ret)
  {
    perror("sendto() failed\n");
  }

  struct sockaddr_ll client = {0};
  socklen_t len = sizeof(client);
  struct udphdr *udph_recv = NULL;
  char recv_message[128] = {0};
  
  while(1)
  {
      ret = recvfrom(sockfd, &recv_message, sizeof(recv_message), 0, (struct sockaddr *)&client, &len);
      if (-1 == ret)
      {
        perror("recvfrom() failed\n");
      }

      udph_recv = (struct udphdr *)(recv_message + IP_OFFSET + ETH_OFFSET);
      
      if (udph_recv->uh_sport == htons(DST_PORT)) 
      {
        printf("Server answered: %s\n", recv_message + OFFSET);
        break;
      }
  }

  return 0;
}
