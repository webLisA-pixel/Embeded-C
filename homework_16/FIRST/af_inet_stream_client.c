#include "af_inet_stream.h"

int main()
{
	int serverfd;
	int ret;
	char buffer[7];
	struct sockaddr_in serv;
	//назначения порта и IP локального интерфейса
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//установление связи с серверной стороной
	ret = connect(serverfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("client connect: ");
		exit(1);
	}
	//отправка сообщения в сокет
	strcpy(buffer, "Hello!");
	ret = write(serverfd, buffer, sizeof(buffer));
	if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	//получение ответного сообщения из сокета
	memset(buffer, 0, sizeof(buffer));
	ret = read(serverfd, buffer, sizeof(buffer));
	printf("Client send: %s\n", buffer);
	close(serverfd);
	exit(0);
}