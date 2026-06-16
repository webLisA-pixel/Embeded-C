#include "af_inet.h"

int main()
{
	int serverfd;
	int ret;
	char buffer[7];
	socklen_t len;
	time_t current_time;
	struct tm *date;
	struct sockaddr_in serv, cli;
	serverfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48889);
	ret = connect(serverfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("client connect: ");
		exit(1);
	}
	strcpy(buffer, "Date");
	len=sizeof(struct sockaddr_in);
	ret = sendto(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serv, len);
	if(ret<0)
	{
		perror("Error send: ");
		exit(1);
	}
	ret = recvfrom(serverfd, &current_time, sizeof(current_time), 0, (struct sockaddr*)&serv, (unsigned int*) &len);
	if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	date=localtime(&current_time);
	printf("Server date: %d %d %d", date->tm_mday, date->tm_mon, date->tm_year);
	close(serverfd);
	exit(0);
}