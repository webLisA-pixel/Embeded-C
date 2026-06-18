#include "raw.h"

int main()
{
	int serverfd;
	int ret;
	int port;
	socklen_t len;
	struct sockaddr_ll serv;
	struct sockaddr_in cli;
	char input_string[PAYLOAD_SIZE];
	char *payload_rcv, *payload_snd, packet_rcv[PACKET_SIZE], packet_snd[PACKET_SIZE];
	serverfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	memset(packet_snd, 0 , PACKET_SIZE);
	//создание структуры заголовков пакета
	struct ether_header *ethheader_snd = (struct ether_header*)packet_snd;
	struct iphdr *ipheader_snd = (struct iphdr *)(packet_snd+sizeof(struct ether_header));
	struct udphdr *udpheader_snd = (struct udphdr *)(packet_snd+sizeof(struct ether_header)+sizeof(struct iphdr));
	payload_snd = packet_snd+sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct udphdr);
	struct ether_header *ethheader_rcv = (struct ether_header*)packet_rcv;
	struct iphdr *ipheader_rcv = (struct iphdr *)(packet_rcv+sizeof(struct ether_header));
	struct udphdr *udpheader_rcv = (struct udphdr *)(packet_rcv+sizeof(struct ether_header)+sizeof(struct iphdr));
	payload_rcv = packet_rcv+sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct udphdr);
	//заполнение загаловка канального уровня
	sscanf(SERVER_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &ethheader_snd->ether_dhost[0], &ethheader_snd->ether_dhost[1], &ethheader_snd->ether_dhost[2], &ethheader_snd->ether_dhost[3], &ethheader_snd->ether_dhost[4], &ethheader_snd->ether_dhost[5]);
	sscanf(CLIENT_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &ethheader_snd->ether_shost[0], &ethheader_snd->ether_shost[1], &ethheader_snd->ether_shost[2], &ethheader_snd->ether_shost[3], &ethheader_snd->ether_shost[4], &ethheader_snd->ether_shost[5]);
	ethheader_snd->ether_type = htons(ETH_P_IP);
	memset(&serv, 0, sizeof(serv));
	serv.sll_family = AF_PACKET;
	serv.sll_protocol = htons(ETH_P_ALL);
	serv.sll_ifindex = if_nametoindex("eth2");
	serv.sll_pkttype = PACKET_HOST;
	serv.sll_halen = 6;
	strncpy(serv.sll_addr, ethheader_snd->ether_dhost, 6);
	//заполнение загаловка сетевого уровня
	ipheader_snd->ihl=5;
	ipheader_snd->version=4;
	ipheader_snd->tos=0;
	int length = PAYLOAD_SIZE + sizeof(struct iphdr) + sizeof(struct udphdr);
	ipheader_snd->tot_len=htons(length);
	ipheader_snd->ttl=64;
	ipheader_snd->id=0;
	ipheader_snd->frag_off=0;
	ipheader_snd->protocol=17;
	ipheader_snd->saddr=inet_addr(CLIENT_IP);
	ipheader_snd->daddr=inet_addr(SERVER_IP);
	ipheader_snd->check=0;
	ipheader_snd->check=checksum((unsigned short *)ipheader_snd, sizeof(struct iphdr));
	//заполнение загаловка транспортного уровня
	udpheader_snd->source=htons(CLIENT_PORT);
	udpheader_snd->dest=htons(SERVER_PORT);
	udpheader_snd->len=htons(sizeof(struct udphdr)+PAYLOAD_SIZE);
	udpheader_snd->check=0;
	strncpy(payload_snd, "Connect", PACKET_SIZE);
	//отправка первичного пакета серверу
	ret = sendto(serverfd, packet_snd, PACKET_SIZE, 0, (struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("Error send: ");
		exit(1);
	}
	memset(&cli, 0, sizeof(cli));
	len=sizeof(struct sockaddr_in);
	while(1)
	{
		//получение нового порта для обмена сообщениями
		memset(packet_rcv, 0 , PACKET_SIZE);
		ret = recvfrom(serverfd, &packet_rcv, PACKET_SIZE, 0, (struct sockaddr*)&cli, (unsigned int*) &len);
		if(ret<0)
		{
			perror("Error read: ");
			exit(1);
		}
		if(udpheader_rcv->source == htons(SERVER_PORT))
		{	
			printf("Server send port: %s\n", payload_rcv);
			sscanf(payload_rcv, "%d", &port);
			close(serverfd);
			break;
		}
	}
	//открытие нового сокета
	serverfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//переопределение порта транспортного уровня
	udpheader_snd->dest=htons(port);
	udpheader_snd->len=htons(sizeof(struct udphdr)+PAYLOAD_SIZE);
	while(1)
	{
		printf("Input string: ");
		scanf("%s", input_string);
		strncpy(payload_snd, input_string, PAYLOAD_SIZE);
		ret = sendto(serverfd, packet_snd, PACKET_SIZE, 0, (struct sockaddr *) &serv, sizeof(serv));
		if(ret<0)
		{
			perror("Error send: ");
			exit(1);
		}
		if(strcmp(payload_snd, "exit")==0)
			break;
		memset(&cli, 0, sizeof(cli));
		len=sizeof(struct sockaddr_in);
		//обмен сообщениями с сервером
		while(1)
		{
			memset(packet_rcv, 0 , PACKET_SIZE);
			ret = recvfrom(serverfd, &packet_rcv, PACKET_SIZE, 0, (struct sockaddr*)&cli, (unsigned int*) &len);
			if(ret<0)
			{
				perror("Error read: ");
				exit(1);
			}
			if(udpheader_rcv->source == htons(port))
			{	
				printf("Server send: %s\n", payload_rcv);
				break;
			}
		}
	}
	close(serverfd);
	exit(0);
}