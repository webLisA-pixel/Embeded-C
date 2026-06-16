#include "broadcast.h"

int main()
{
	int sockfd;
	int ret, flag;
	char buffer[7];
	socklen_t len;
	struct sockaddr_in serv;
	struct sockaddr_in cli;
	memset(&serv, 0, sizeof(serv));
	//назначение параметров и открытие сокета
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	flag =1;
	//добавления параметра для передачи breoadcast пакета
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &flag, sizeof(flag));
	if(sockfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	memset(buffer, 0, sizeof(buffer));
	strncpy(buffer, "Hi!", sizeof(buffer));
	len = sizeof(serv);
	for(int i = 0; i<10; i++)
	{
		//отправка broadcast пакета
		ret = sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serv, len);
		if(ret<0)
		{
			perror("Error sendto: ");
			exit(1);
		}
	}
	close(sockfd);
	exit(0);
}