#include "multicast.h"

int main()
{
	int serverfd;
	int ret;
	struct ip_mreqn mreqn;
	char buffer[7];
	socklen_t len;
	struct sockaddr_in serv, cli;
	//назначение параметров и открытие сокета для приему multicast
	serverfd = socket(AF_INET, SOCK_DGRAM, 0);
	mreqn.imr_multiaddr.s_addr = inet_addr(SERVER_IP);
	mreqn.imr_address.s_addr = INADDR_ANY;
	//назначение сокету опции для приема multicast
	setsockopt(serverfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreqn, sizeof(mreqn));
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(48888);
	//привязка параметров сокету
	ret = bind(serverfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("Error bind: ");
		exit(1);
	}
	for(int i = 0; i<10;i++)
	{
		//прием multicast пакета
		ret = recvfrom(serverfd, &buffer, sizeof(buffer), 0, (struct sockaddr*)NULL, NULL);
		if(ret<0)
		{
			perror("Error read: ");
			exit(1);
		}
		printf("Server send: %s\n", buffer);
	}
	close(serverfd);
	exit(0);
}