#include "multicast.h"

int main()
{
	int sockfd;
	int ret, flag;
	char buffer[7];
	socklen_t len;
	struct sockaddr_in serv;
	struct sockaddr_in cli;
	//назначение параметров и открытие сокета для отправки multicast трафика
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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
		//отправка пакета на multicast адрес
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