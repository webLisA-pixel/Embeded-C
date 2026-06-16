#include "af_inet_stream.h"

int main()
{
	int serverfd, serverfd2;
	int ret;
	int port=0;
	char buffer[7];
	struct sockaddr_in serv;
	time_t current_time;
	struct tm *date;
	memset(&serv, 0, sizeof(serv));
	//назачение первичных параметров для соединения с сервером
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	//открытие сокета
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//подключенире к серверу для получения нового порта для передачи сообщений
	ret = connect(serverfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("client connect: ");
		exit(1);
	}
	//получение нового порта от сервера
	ret = recv(serverfd, &port, sizeof(port), 0);
	if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	printf("recive port from server: %d\n", port);
	//закрытие первичного сокета
	close(serverfd);
	//изменени япараметров для подключения к серверу
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(port);
	//открытие нового сокета
	serverfd2 = socket(AF_INET, SOCK_STREAM, 0);
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//подключение к серверу с новыми параметрами
	ret = connect(serverfd2, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("client connect: ");
		exit(1);
	}
	//передача запроса времени серверу
	strncpy(buffer, "Date", sizeof(buffer));
	ret = send(serverfd2, &buffer, sizeof(buffer), 0);
	if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	//получение времени сервера
	memset(&current_time, 0, sizeof(current_time));
	ret = recv(serverfd2, &current_time, sizeof(current_time), 0);
	date=localtime(&current_time);
	printf("Server date in port %d: %d %d %d", port, date->tm_mday, date->tm_mon, date->tm_year);
	close(serverfd2);
	exit(0);
}