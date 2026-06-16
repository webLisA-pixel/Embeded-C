#include "af_inet.h"

int main()
{
	int serverfd;
	int ret;
	char buffer[7];
	time_t current_time;
	struct tm *date;
	struct sockaddr_in serv;
	//открытие сокета и назначение параметров
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
	//поделючение к серверу
	ret = connect(serverfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("client connect: ");
		exit(1);
	}
	strcpy(buffer, "Date");
	//отправка запроса времени серверу
	ret = write(serverfd, buffer, sizeof(buffer));
	if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	//получение времени сервера
	ret = read(serverfd, &current_time, sizeof(current_time));
	date=localtime(&current_time);
	printf("Server date: %d %d %d", date->tm_mday, date->tm_mon, date->tm_year);
	close(serverfd);
	exit(0);
}