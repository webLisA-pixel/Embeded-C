#include "af_inet_stream.h"

struct for_thread
{
	int port;
};

void *proces(void* arg)
{
	struct for_thread *threads = (struct for_thread *) arg;
	int processfd, ret, client;
	struct sockaddr_in serv, cli;
	socklen_t len;
	time_t current_time;
	char buffer[7];
	memset(&serv, 0, sizeof(serv));
	//присвоение новых параметров для обработки запросов от клиента
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(threads->port);
	processfd = socket(AF_INET, SOCK_STREAM, 0);
	if(processfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//присвоение параметров сокету
	ret = bind(processfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("Error binding: ");
		exit(1);
	}
	//прослушка сокета
	ret = listen(processfd, 5);
	if(ret<0)
	{
		perror("Error listening: ");
		exit(1);
	}
	//установление связи с клиентом
	memset(&cli, 0, sizeof(struct sockaddr_in));
	len=sizeof(struct sockaddr_in);
	client = accept(processfd, (struct sockaddr *) &cli, &len);
	if(client<0)
	{
		perror("client connect: ");
		exit(1);
	}
	//прием запроса от клиента
	ret = recv(client, &buffer, sizeof(buffer), 0);
	if(ret<0)
	{
		perror("Error read: ");
		exit(1);
	}
	//отправка серверного времени клиенту
	if(strcmp(buffer, "Date")==0)
	{
		current_time = time(NULL);
		ret = send(client, &current_time, sizeof(current_time), 0);
		if(ret<0)
		{
			perror("Error read: ");
			exit(1);
		}
	}
	close(client);
	close(processfd);
}

void *registr(void* arg)
{
	pthread_t thread[10];
	int sockfd;
	int client;
	int ret, n, port;
	struct for_thread threads;
	socklen_t len;
	char buffer[7];
	struct sockaddr_in serv, cli;
	memset(&serv, 0, sizeof(serv));
	//назначение параметров и открытие сокета для согласования нового порта для обработки запросов
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	port = 59999;
	n = 0;
	if(sockfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//привязка параметров сокету
	ret = bind(sockfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("Error binding: ");
		exit(1);
	}
	while (n<10)
	{
		//прослушка сокета
		ret = listen(sockfd, 5);
		if(ret<0)
		{
			perror("Error listening: ");
			exit(1);
		}
		//установка связи с клиентом
		memset(&cli, 0, sizeof(struct sockaddr_in));
		len=sizeof(cli);
		client = accept(sockfd, (struct sockaddr *) &cli, &len);
		if(client<0)
		{
			perror("client connect: ");
			exit(1);
		}
		threads.port = port;
		pthread_create(&thread[n], NULL, proces, (void *) &threads);
		//отправка серверного времени клиенту
		ret = send(client, &port, sizeof(port), 0);
		if(ret<0)
		{
			perror("Error read: ");
			exit(1);
		}
		close(client);
		n ++;
		port ++;
	}
	exit(0);
}

int main()
{
	pthread_t thread;
	pthread_create(&thread, NULL, registr, NULL);
	pthread_join(thread, NULL);
}