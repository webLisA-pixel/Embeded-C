#include "af_local_stream.h"

int main()
{
	int sockfd;
	int client;
	int ret;
	char buffer[7];
	struct sockaddr_un serv;
	//назначение параметвро для передачи данных
	memset(&serv, 0, sizeof(serv));
	serv.sun_family = AF_LOCAL;
	strncpy(serv.sun_path, SOCK_NAME, sizeof(serv.sun_path)-1);
	//открытие сокета
	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//присвоение параметров сокету
	ret = bind(sockfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("Error binding: ");
		exit(1);
	}
	//включение прослушки сокета
	ret = listen(sockfd, 5);
	if(ret<0)
	{
		perror("Error listening: ");
		exit(1);
	}
	//установление соединения с клитентом
	client = accept(sockfd, NULL, NULL);
	if(client<0)
	{
		perror("client connect: ");
		exit(1);
	}
	//получение сообщения из сокета
	ret = read(client, buffer, sizeof(buffer));
	if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	printf("Client send: %s\n", buffer);
	//отправка сообщения в сокет
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "Hi!");
	ret = write(client, buffer, sizeof(buffer));
	sleep(2);
	close(client);
	exit(0);
}