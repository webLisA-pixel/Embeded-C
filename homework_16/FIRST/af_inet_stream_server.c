#include "af_inet_stream.h"

int main()
{
	int sockfd;
	int client;
	int ret, len;
	char buffer[7];
	struct sockaddr_in serv, cli;
	//назначения порта и IP локального интерфейса
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//присвоение сокету параметров для передачи данных
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
	len=sizeof(cli);
	//установление связи с клиентской стороной
	client = accept(sockfd, (struct sockaddr *) &cli, &len);
	if(client<0)
	{
		perror("client connect: ");
		exit(1);
	}
	ret = read(client, buffer, sizeof(buffer));
	if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	printf("Client send: %s\n", buffer);
	memset(buffer, 0, sizeof(buffer));
	strcpy(buffer, "Hi!");
	ret = write(client, buffer, sizeof(buffer));
	close(client);
	exit(0);
}