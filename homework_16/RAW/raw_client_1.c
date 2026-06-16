#include "raw.h"

int main()
{
	int serverfd;
	int ret;
	char *payload, packet[PACKET_SIZE];
	socklen_t len;
	memset(packet, 0 , PACKET_SIZE);
	struct udphdr *header = (struct udphdr *)packet;
	struct sockaddr_in serv, cli;
	//назначение параметров и открытие сокета
	serverfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	//назначение UDP заголовка для пакета
	payload = packet+sizeof(struct udphdr);
	header->source=htons(CLIENT_PORT);
	header->dest=htons(SERVER_PORT);
	//header->len=htons(PACKET_SIZE);
	header->len=htons(sizeof(struct udphdr)+PAYLOAD_SIZE);
	header->check=0;
	strncpy(payload, "Hello!\n", PACKET_SIZE);
	//отправка UDP пакета
	memset(&cli, 0, sizeof(cli));
	len=sizeof(struct sockaddr_in);
	ret = sendto(serverfd, packet, PACKET_SIZE, MSG_CONFIRM, (struct sockaddr *) &serv, len);
	if(ret<0)
	{
		perror("Error send: ");
		exit(1);
	}
    struct udphdr* header_rcv = (struct udphdr *)(packet + sizeof(struct iphdr));
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
		if(header_rcv->source == htons(SERVER_PORT))
			break;
	}
	printf("%s\n", packet);
	close(serverfd);
	exit(0);
}