#include "raw.h"

int main()
{
	int serverfd;
	int ret;
	socklen_t len;
	char *payload, packet[PACKET_SIZE];
	//назначение параметров и открытие сокета
	memset(packet, 0 , PACKET_SIZE);
	struct iphdr *ipheader = (struct iphdr*)packet;
	struct udphdr *udpheader = (struct udphdr *)(packet+sizeof(struct iphdr));
	struct sockaddr_in serv, cli;
	serverfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	int flag =1;
	//добавление опции сокету что будет изменен ip заголовок
	setsockopt(serverfd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag));
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	//назначение параметров ip заголовка
	payload = packet+sizeof(struct iphdr)+sizeof(struct udphdr);
	ipheader->version=4;
	ipheader->ihl=5;
	ipheader->tos=0;
	ipheader->tot_len=0;
	ipheader->ttl=64;
	ipheader->id=0;
	ipheader->frag_off=0;
	ipheader->protocol=17;
	ipheader->check=0;
	ipheader->saddr=inet_addr(CLIENT_IP);
	ipheader->daddr=inet_addr(SERVER_IP);
	//назначение параметров UDP заголовка
	udpheader->source=htons(CLIENT_PORT);
	udpheader->dest=htons(SERVER_PORT);
	udpheader->len=htons(sizeof(struct udphdr)+PAYLOAD_SIZE);
	udpheader->check=0;
	strncpy(payload, "Hello!\n", PACKET_SIZE);
	memset(&cli, 0, sizeof(cli));
	len=sizeof(struct sockaddr_in);
	//отправка пакета в сторону сервера
	ret = sendto(serverfd, packet, PACKET_SIZE, MSG_CONFIRM, (struct sockaddr *) &serv, len);
	if(ret<0)
	{
		perror("Error send: ");
		exit(1);
	}
    struct udphdr* udpheader_rcv = (struct udphdr *)(packet + sizeof(struct iphdr));
    char* payload_rcv = packet + sizeof(struct iphdr) + sizeof(struct udphdr);
	while(1)
	{
		//прием пакета от сервера
		ret = recvfrom(serverfd, &packet, PACKET_SIZE, MSG_WAITALL, (struct sockaddr*)&cli, (unsigned int*) &len);
		if(ret<0)
		{
			perror("Error read: ");
			exit(1);
		}
		if(udpheader_rcv->source == htons(SERVER_PORT))
			break;
	}
	printf("%s\n", packet);
	close(serverfd);
	exit(0);
}