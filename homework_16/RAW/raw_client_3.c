#include "raw.h"

unsigned short checksum(unsigned short *header, int len)
{
	int sum = 0;
    short tmp;
    unsigned short *ptr = header;
    unsigned short chksum;
    for(int i = 0; i < (len / 2); i++){
      sum += *ptr;
      ptr++;
    }
    tmp = (sum >> 16);
    chksum =  ~((sum & 0xffff) + tmp);
    return chksum;
}

int main()
{
	int serverfd;
	int ret;
	socklen_t len;
	struct sockaddr_ll serv;
	struct sockaddr_in cli;
	char *payload, packet[PACKET_SIZE];
	//назначение параметров и открытие сокета
	serverfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	memset(packet, 0 , PACKET_SIZE);
	//назначение заголовков канального, сетевого и транспортного уровней
	struct ether_header *ethheader = (struct ether_header*)packet;
	struct iphdr *ipheader = (struct iphdr *)(packet+sizeof(struct ether_header));
	struct udphdr *udpheader = (struct udphdr *)(packet+sizeof(struct ether_header)+sizeof(struct iphdr));
	payload = packet+sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct udphdr);
	//конвертация mac адресов и указание поля type
	sscanf(SERVER_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &ethheader->ether_dhost[0], &ethheader->ether_dhost[1], &ethheader->ether_dhost[2], &ethheader->ether_dhost[3], &ethheader->ether_dhost[4], &ethheader->ether_dhost[5]);
	sscanf(CLIENT_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &ethheader->ether_shost[0], &ethheader->ether_shost[1], &ethheader->ether_shost[2], &ethheader->ether_shost[3], &ethheader->ether_shost[4], &ethheader->ether_shost[5]);
	ethheader->ether_type = htons(ETH_P_IP);
	memset(&serv, 0, sizeof(serv));
	serv.sll_family = AF_PACKET;
	serv.sll_protocol = htons(ETH_P_ALL);
	serv.sll_ifindex = if_nametoindex("eth2");
	serv.sll_pkttype = PACKET_HOST;
	serv.sll_halen = 6;
	strncpy(serv.sll_addr, ethheader->ether_dhost, 6);
	//заполнение заголовка сетевого уровня
	ipheader->ihl=5;
	ipheader->version=4;
	ipheader->tos=0;
	int length = PAYLOAD_SIZE + sizeof(struct iphdr) + sizeof(struct udphdr);
	ipheader->tot_len=htons(length);
	ipheader->ttl=64;
	ipheader->id=0;
	ipheader->frag_off=0;
	ipheader->protocol=17;
	ipheader->saddr=inet_addr(CLIENT_IP);
	ipheader->daddr=inet_addr(SERVER_IP);
	ipheader->check=0;
	ipheader->check=checksum((unsigned short *)ipheader, sizeof(struct iphdr));
	//заполнение полей транспортного уровня
	udpheader->source=htons(CLIENT_PORT);
	udpheader->dest=htons(SERVER_PORT);
	udpheader->len=htons(sizeof(struct udphdr)+PAYLOAD_SIZE);
	udpheader->check=0;
	strncpy(payload, "Hello!\n", PACKET_SIZE);	
	//отправка пакета серверу							   
	ret = sendto(serverfd, packet, PACKET_SIZE, 0, (struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("Error send: ");
		exit(1);
	}
	memset(&cli, 0, sizeof(cli));
	len=sizeof(struct sockaddr_in);
	while(1)
	{
		//прием пакета от сервера
		ret = recvfrom(serverfd, &packet, PACKET_SIZE, 0, (struct sockaddr*)&cli, (unsigned int*) &len);
		if(ret<0)
		{
			perror("Error read: ");
			exit(1);
		}
		if(udpheader->source == htons(SERVER_PORT))
			printf("Server send: %s", payload);
			break;
	}
	close(serverfd);
	exit(0);
}