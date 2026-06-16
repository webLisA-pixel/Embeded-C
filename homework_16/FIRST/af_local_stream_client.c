#include "af_local_stream.h"

int main()
{
	int serverfd;
	int ret;
	char buffer[7];
	struct sockaddr_un serv;
	memset(&serv, 0, sizeof(serv));
	//назначение параметров для локальной передачи данных
	serv.sun_family = AF_LOCAL;
	strncpy(serv.sun_path, SOCK_NAME, sizeof(serv.sun_path)-1);
	//открытие сокета
	serverfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//установление подключения с сервером
	ret = connect(serverfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("client connect: ");
		exit(1);
	}
	strcpy(buffer, "Hello!");
	//отправка сообщения в сокет
	ret = write(serverfd, buffer, sizeof(buffer));
	if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	//получение сообщения из сокета
	memset(buffer, 0, sizeof(buffer));
	ret = read(serverfd, buffer, sizeof(buffer));
	printf("Client send: %s\n", buffer);
	close(serverfd);
	exit(0);
}