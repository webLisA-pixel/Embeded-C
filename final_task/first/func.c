#include "raw.h"

struct info_threads {
	int id;
	int state;		//0-not create, 1-free, 2-work
};
//Функция подсчета checksum
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
//функция управления сокетами
void *server_control()
{	
	int serverfd;
	int ret, length;
	socklen_t len;
	struct info_threads thread[5];
	pthread_t num_thread[5];
	struct sockaddr_ll serv, cli;
	char *payload_rcv, *payload_snd, packet_rcv[PACKET_SIZE], packet_snd[PACKET_SIZE];
	//создание структуры о занятости порта 
	for(int i=0; i<5; i++)
	{
		thread[i].id=i;
		thread[i].state=0;
	}
	serverfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	memset(packet_snd, 0 , PACKET_SIZE);
	//заполнение структуры пакета
	struct ether_header *ethheader_snd = (struct ether_header*)packet_snd;
	struct iphdr *ipheader_snd = (struct iphdr *)(packet_snd+sizeof(struct ether_header));
	struct udphdr *udpheader_snd = (struct udphdr *)(packet_snd+sizeof(struct ether_header)+sizeof(struct iphdr));
	payload_snd = packet_snd+sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct udphdr);
	struct ether_header *ethheader_rcv = (struct ether_header*)packet_rcv;
	struct iphdr *ipheader_rcv = (struct iphdr *)(packet_rcv+sizeof(struct ether_header));
	struct udphdr *udpheader_rcv = (struct udphdr *)(packet_rcv+sizeof(struct ether_header)+sizeof(struct iphdr));
	payload_rcv = packet_rcv+sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct udphdr);
	//назначение загаловка канального уровня
	sscanf(SERVER_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &ethheader_snd->ether_shost[0], &ethheader_snd->ether_shost[1], &ethheader_snd->ether_shost[2], &ethheader_snd->ether_shost[3], &ethheader_snd->ether_shost[4], &ethheader_snd->ether_shost[5]);
	ethheader_snd->ether_type = htons(ETH_P_IP);
	memset(&serv, 0, sizeof(serv));
	serv.sll_family = AF_PACKET;
	serv.sll_protocol = htons(ETH_P_ALL);
	serv.sll_ifindex = if_nametoindex("eth1");
	serv.sll_pkttype = PACKET_HOST;
	serv.sll_halen = 6;
	//заполнение заголовка сетевого уровня
	ipheader_snd->ihl=5;
	ipheader_snd->version=4;
	ipheader_snd->tos=0;
	ipheader_snd->ttl=64;
	ipheader_snd->id=0;
	ipheader_snd->frag_off=0;
	ipheader_snd->protocol=17;
	ipheader_snd->saddr=inet_addr(SERVER_IP);
	ipheader_snd->check=0;
	//заполнение заголовка транспортного уровня
	udpheader_snd->source=htons(SERVER_PORT);
	udpheader_snd->len=htons(sizeof(struct udphdr)+PAYLOAD_SIZE);
	udpheader_snd->check=0;
	while(1)
	{
		memset(packet_rcv, 0 , PACKET_SIZE);
		memset(&cli, 0, sizeof(cli));
		len=sizeof(struct sockaddr_in);
		//прием первичного сообщения от клиента
		ret = recvfrom(serverfd, &packet_rcv, PACKET_SIZE, 0, (struct sockaddr*)&cli, (unsigned int*) &len);
		if(ret<0)
		{
			perror("Error read: ");
			exit(1);
		}
		if(udpheader_rcv->source == htons(CLIENT_PORT))
		{
			if(strcmp(payload_rcv, "Connect")==0)
			{
				//назначение адресов для отправки пакета клиенту
				for(int i=0; i<6; i++)
				{
					ethheader_snd->ether_shost[i]=ethheader_rcv->ether_dhost[i];
					ethheader_snd->ether_dhost[i]=ethheader_rcv->ether_shost[i];
				}
				length = PAYLOAD_SIZE + sizeof(struct iphdr) + sizeof(struct udphdr);
				ipheader_snd->tot_len=htons(length);
				ipheader_snd->daddr=ipheader_rcv->saddr;
				ipheader_snd->check=checksum((unsigned short *)ipheader_snd, sizeof(struct iphdr));
				udpheader_snd->dest=udpheader_rcv->source;
				for(int i=0; i<5; i++)
				{
					if(thread[i].state==0)
					{
						//создание потока и отправка нового порта для общения с сервером клиенту
						pthread_create(&num_thread[i], NULL, processed, (void *)&thread[i]);
						thread[i].state=2;
						int port = START_SERVER_PORT+thread[i].id;
						sprintf(payload_snd, "%d", port);
						ret = sendto(serverfd, packet_snd, PACKET_SIZE, 0, (struct sockaddr *) &cli, sizeof(cli));
						if(ret<0)
						{
							perror("Error send: ");
							exit(1);
						}
						break;
					}
					else if(thread[i].state==1)
					{
						thread[i].state=2;
						int port = START_SERVER_PORT+thread[i].id;
						sprintf(payload_snd, "%d", port);
						ret = sendto(serverfd, packet_snd, PACKET_SIZE, 0, (struct sockaddr *) &cli, sizeof(cli));
						if(ret<0)
						{
							perror("Error send: ");
							exit(1);
						}
						break;
					}
					if(i==4)
						printf("Server if busy");
				}
			}
		}
	}
	for(int i=0; i<5;i++)
		pthread_join(num_thread[i], NULL);
	close(serverfd);
	exit(0);
}

//функция для передачи сообщений между клиентом и сервером
void *processed(void *arg)
{
	struct info_threads *thread = (struct info_threads *)arg;
	int processfd;
	int ret, length;
	socklen_t len;
	int msg_count=1;
	char str_count[10];
	struct sockaddr_ll serv, cli;
	char *payload_rcv, *payload_snd, packet_rcv[PACKET_SIZE], packet_snd[PACKET_SIZE];
	processfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(processfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	memset(packet_snd, 0 , PACKET_SIZE);
	//заполнение структуры пакета
	struct ether_header *ethheader_snd = (struct ether_header*)packet_snd;
	struct iphdr *ipheader_snd = (struct iphdr *)(packet_snd+sizeof(struct ether_header));
	struct udphdr *udpheader_snd = (struct udphdr *)(packet_snd+sizeof(struct ether_header)+sizeof(struct iphdr));
	payload_snd = packet_snd+sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct udphdr);
	struct ether_header *ethheader_rcv = (struct ether_header*)packet_rcv;
	struct iphdr *ipheader_rcv = (struct iphdr *)(packet_rcv+sizeof(struct ether_header));
	struct udphdr *udpheader_rcv = (struct udphdr *)(packet_rcv+sizeof(struct ether_header)+sizeof(struct iphdr));
	payload_rcv = packet_rcv+sizeof(struct ether_header)+sizeof(struct iphdr)+sizeof(struct udphdr);
	//заполнение загаловка канального уровня
	sscanf(SERVER_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &ethheader_snd->ether_shost[0], &ethheader_snd->ether_shost[1], &ethheader_snd->ether_shost[2], &ethheader_snd->ether_shost[3], &ethheader_snd->ether_shost[4], &ethheader_snd->ether_shost[5]);
	ethheader_snd->ether_type = htons(ETH_P_IP);
	memset(&serv, 0, sizeof(serv));
	serv.sll_family = AF_PACKET;
	serv.sll_protocol = htons(ETH_P_ALL);
	serv.sll_ifindex = if_nametoindex("eth1");
	serv.sll_pkttype = PACKET_HOST;
	serv.sll_halen = 6;
	//заполнение загаловка сетевого уровня
	ipheader_snd->ihl=5;
	ipheader_snd->version=4;
	ipheader_snd->tos=0;
	ipheader_snd->ttl=64;
	ipheader_snd->id=0;
	ipheader_snd->frag_off=0;
	ipheader_snd->protocol=17;
	ipheader_snd->saddr=inet_addr(SERVER_IP);
	ipheader_snd->check=0;
	////заполнение загаловка транспортного уровня
	udpheader_snd->source=htons(START_SERVER_PORT+thread->id);
	udpheader_snd->len=htons(sizeof(struct udphdr)+PAYLOAD_SIZE);
	udpheader_snd->check=0;
	memset(&cli, 0, sizeof(cli));
	while(1)
	{
		memset(packet_rcv, 0 , PACKET_SIZE);
		memset(payload_snd, 0 ,PACKET_SIZE);
		len=sizeof(struct sockaddr_in);
		while(1)
		{
			//прием первого сообщения от клиента
			ret = recvfrom(processfd, &packet_rcv, PACKET_SIZE, 0, (struct sockaddr*)&cli, (unsigned int*) &len);
			if(ret<0)
			{
				perror("Error read: ");
				exit(1);
			}
			//заполнение заголовов для отправки сообщений обратно клиенту
			if(ipheader_rcv->saddr==inet_addr(CLIENT_IP) && udpheader_rcv->source == htons(CLIENT_PORT) && udpheader_rcv->dest==htons(START_SERVER_PORT+thread->id))
				break;
		}
		if(strcmp(payload_rcv, "exit")==0)
		{	
			msg_count=1;
			thread->state=1;
			continue;
		}
		for(int i=0; i<6; i++)
		{
			ethheader_snd->ether_shost[i]=ethheader_rcv->ether_dhost[i];
			ethheader_snd->ether_dhost[i]=ethheader_rcv->ether_shost[i];
		}
		length = PAYLOAD_SIZE + sizeof(struct iphdr) + sizeof(struct udphdr);
		ipheader_snd->tot_len=htons(length);
		ipheader_snd->daddr=ipheader_rcv->saddr;
		ipheader_snd->check=checksum((unsigned short *)ipheader_snd, sizeof(struct iphdr));
		udpheader_snd->dest=udpheader_rcv->source;
		//дополнение номера сообщения к полученному от клиента
		strcat(payload_snd, payload_rcv);
		strcat(payload_snd, " ");
		memset(str_count, 0, sizeof(str_count));
		sprintf(str_count, "%d", msg_count);
		strcat(payload_snd, str_count);
		strcat(payload_snd, (char *)&msg_count);
		//отправка клиенту нового сообщения
		ret = sendto(processfd, packet_snd, PACKET_SIZE, 0, (struct sockaddr *) &cli, sizeof(cli));
		if(ret<0)
		{
			perror("Error send: ");
			exit(1);
		}
		msg_count++;
	}
	close(processfd);
}