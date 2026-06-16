#include "af_inet_stream.h"

struct for_thread
{
	int average;
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
	//назначение новых параметров и открытие сокета
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(threads->port);
	processfd = socket(AF_INET, SOCK_STREAM, 0);
	if(processfd<0)
	{
		perror("Socket create: ");
		exit(1);
	}
	//привязка параметров к сокету
	ret = bind(processfd, (const struct sockaddr *) &serv, sizeof(serv));
	if(ret<0)
	{
		perror("Error binding: ");
		exit(1);
	}
	while(1)
	{
		//прослушка сокета
		ret = listen(processfd, 5);
		if(ret<0)
		{
			perror("Error listening: ");
			exit(1);
		}
		memset(&cli, 0, sizeof(struct sockaddr_in));
		len=sizeof(struct sockaddr_in);
		//установление связи с клиентом
		client = accept(processfd, (struct sockaddr *) &cli, &len);
		if(client<0)
		{
			perror("client connect: ");
			exit(1);
		}
		threads->average = 1;
		//прием запроса от сервера
		ret = recv(client, &buffer, sizeof(buffer), 0);
		if(ret<0)
		{
			perror("Error read: ");
			exit(1);
		}
		//отправка времени сервера
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
		sleep(5);
		close(client);
		threads->average = 0;
	}
	close(processfd);
}

void *registr(void* arg)
{
	pthread_t thread[5];
	int sockfd;
	int client;
	int ret, n, port;
	struct for_thread threads[5];
	socklen_t len;
	char buffer[7];
	struct sockaddr_in serv, cli;
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv.sin_port = htons(48888);
	port = 59999;
	n = 5;
	int step = n;
	//открытие первичного сокета для согласования новго порта для связи
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
	//создание потоков для обработки запросов
	for(int i = 0; i<n; i++)
	{
		threads[i].port = port;
		threads[i].average = 0;
		pthread_create(&thread[i], NULL, proces, (void *) &threads[i]);
		port ++;
	}
	while (step!=0)
	{
		ret = listen(sockfd, 5);
		if(ret<0)
		{
			perror("Error listening: ");
			exit(1);
		}
		memset(&cli, 0, sizeof(struct sockaddr_in));
		len=sizeof(cli);
		//установление связи с клиентской стороной
		client = accept(sockfd, (struct sockaddr *) &cli, &len);
		if(client<0)
		{
			perror("client connect: ");
			exit(1);
		}
		for(int i = 0; i<n; i++)
		{
			if(threads[i].average == 0)
			{
				port = threads[i].port;
				//отправка порта для отправки запроса
				ret = send(client, &port, sizeof(port), 0);
				if(ret<0)
				{
					perror("Error read: ");
					exit(1);
				}
				close(client);
				break;
			}
		}
		step --;
	}
	for(int i = 0; i<n; i++)
	{
		if(threads[i].average != 0)
		{
			sleep(1);
			i--;
		}
	}
	exit(0);
}

int main()
{
	pthread_t thread;
	pthread_create(&thread, NULL, registr, NULL);
	pthread_join(thread, NULL);
}