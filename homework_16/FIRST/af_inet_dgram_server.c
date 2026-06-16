#include "af_inet_dgram.h"

int main()
{
	int sockfd;
	int ret;
	char buffer[7];
	socklen_t len;
	struct sockaddr_in serv;
	struct sockaddr_in cli;
	//назначения порта и IP локального интерфейса
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	//открытие сокета
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//присвоение сокету параметров
	ret = bind(sockfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("Error binding: ");
		exit(1);
	}
	len = sizeof(struct sockaddr_in);
	//прием из сокета сообщения
	ret = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli, (unsigned int*) &len);
	if(ret<0)
	{
		perror("Error recvfrom: ");
		exit(1);
	}
	printf("Client send: %s\n", buffer);
	//отправка ответного сообщения в сокет
	memset(buffer, 0, sizeof(buffer));
	strncpy(buffer, "Hi!", sizeof(buffer));
	ret = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli, len);
	
		if(ret<0)
	{
		perror("Error sendto: ");
		exit(1);
	}
	memset(buffer, 0, sizeof(buffer));
	close(sockfd);
	exit(0);
}