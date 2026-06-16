#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <net/if.h>

#define SERVER_IP "190.160.10.1"
#define CLIENT_IP "190.160.10.2"
#define SERVER_MAC "2a:fb:fd:4c:6f:7a"
#define CLIENT_MAC "82:ae:e1:3a:0b:b4"
#define SERVER_PORT 48888
#define CLIENT_PORT 49888
#define PACKET_SIZE 128
#define PAYLOAD_SIZE 86