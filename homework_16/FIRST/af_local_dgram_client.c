#include "af_local_dgram.h"

int main()
{
	int serverfd;
	int ret;
	char buffer[7];
	struct sockaddr_un serv, cli;
	//открытие сокета
	serverfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	memset(&cli, 0, sizeof(cli));
	cli.sin_family = AF_LOCAL;
	//назначение параметров сокету для локальной работы
	ret = bind(serverfd, (const struct sockaddr *) &cli, sizeof(struct sockaddr_un));
	if(ret<0)
	{
		perror("Error binding: ");
		exit(1);
	}
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_LOCAL;
	strncpy(serv.sun_path, SOCK_NAME, sizeof(serv.sun_path)-1);
	//подключение к серверной стороне
	ret = connect(serverfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("client connect: ");
		exit(1);
	}
	strcpy(buffer, "Hello!");
	//отправка сообщения в сокет
	ret = sendto(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *) NULL, sizeof(serv));
	if(ret<0)
	{
		perror("Error send: ");
		exit(1);
	}
	//прием сообщения из сокета
	memset(buffer, 0, sizeof(buffer));
	ret = recvfrom(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
		if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	printf("Client send: %s\n", buffer);
	close(serverfd);
	exit(0);
}