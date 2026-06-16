#include "raw.h"

int main()
{
	int sockfd;
	int ret;
	char buffer[7];
	socklen_t len;
	int r;
	struct sockaddr_in serv;
	struct sockaddr_in cli;
	//назначения параметров и открытие сокета
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//привязка параметров сокету
	ret = bind(sockfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("Error binding: ");
		exit(1);
	}
	len = sizeof(struct sockaddr_in);
	//прием сообщения от клиента и его обработка
	ret = recvfrom(sockfd, buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *) &cli, (unsigned int*) &len);
	if(ret<0)
	{
		perror("Error recvfrom: ");
		exit(1);
	}
	printf("Client send: %s\n", buffer);
	r=rand()%sizeof(buffer);
	buffer[r]+=r;
	printf("Send to client: %s\n", buffer);
	//отправка клиенту обработанного пакета
	ret = sendto(sockfd, buffer, sizeof(buffer), MSG_CONFIRM, (struct sockaddr *) &cli, len);
	if(ret<0)
	{
		perror("Error sendto: ");
		exit(1);
	}
	memset(buffer, 0, sizeof(buffer));
	close(sockfd);
	exit(0);
}