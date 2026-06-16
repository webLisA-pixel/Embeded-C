#include "af_local_dgram.h"

int main()
{
	int sockfd;
	int ret;
	char buffer[7];
	int len;
	struct sockaddr_un serv, cli;
	memset(&serv, 0, sizeof(serv));
	//назначение параметров для локальной передачи данных
	serv.sun_family = AF_LOCAL;
	strncpy(serv.sun_path, SOCK_NAME, sizeof(serv.sun_path)-1);
	//открытие сокета
	sockfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
	if(sockfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//назначение параметров сокету
	ret = bind(sockfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("Error binding: ");
		exit(1);
	}
	len = sizeof(cli);
	//прием сообщения из сокета
	ret = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli, (unsigned int*) &len);
	if(ret<0)
	{
		perror("Error recvfrom: ");
		exit(1);
	}
	printf("Server send: %s\n", buffer);
	//отправка сообщения в сокет
	memset(buffer, 0, sizeof(buffer));
	strncpy(buffer, "Hi!", sizeof(buffer));
	ret = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli, len);
		if(ret<0)
	{
		perror("Error sendto: ");
		exit(1);
	}
	close(sockfd);
	unlink(SOCK_NAME);
	exit(0);
}