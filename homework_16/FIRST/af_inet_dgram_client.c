#include "af_inet_dgram.h"

int main()
{
	int serverfd;
	int ret;
	char buffer[7];
	socklen_t len;
	struct sockaddr_in serv;
	//Открытие сокета для передачи dgram
	serverfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(serverfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//присвоение порта и IP локального интерфейса
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	//отправка сообщения в сокет
	len=sizeof(struct sockaddr_in);
	strcpy(buffer, "Hello!");
	ret = sendto(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serv, len);
	if(ret<0)
	{
		perror("Error send: ");
		exit(1);
	}
	//прием сообщения из сокета
	ret = recvfrom(serverfd, &buffer, sizeof(buffer), 0, (struct sockaddr*)&serv, (unsigned int*) &len);
	if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	printf("%s\n", buffer);
	close(serverfd);
	exit(0);
}