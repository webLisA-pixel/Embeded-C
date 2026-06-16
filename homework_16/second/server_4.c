#include "af_inet.h"

void *processing(void *arg)
{
	struct epoll_event ev_tcp, ev_udp, events[MAX_EVENTS];
	int sock_tcp, sock_udp;
	int ret, port, client, epollfd, chk;
	socklen_t len;
	time_t current_time;
	char buffer[7];
	struct sockaddr_in serv_tcp, serv_udp, cli;
	//назначение параметров и открытие TCP сокета
	memset(&serv_tcp, 0, sizeof(serv_tcp));
	serv_tcp.sin_family = AF_INET;
	serv_tcp.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv_tcp.sin_port = htons(48888);
	sock_tcp = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_tcp<0)
	{
		perror("Socket create tcp: ");
		exit(1);
	}
	sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_udp<0)
	{
		perror("Socket create udp: ");
		exit(1);
	}
	//Назначение параметров и открытие UDP сокета
	serv_udp.sin_family = AF_INET;
	serv_udp.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv_udp.sin_port = htons(48889);
	port = SERVER_PORT;
	ret = bind(sock_tcp, (const struct sockaddr *) &serv_tcp, sizeof(serv_tcp));
	if(ret<0)
	{
		perror("Error binding tcp: ");
		exit(1);
	}
	ret = bind(sock_udp, (const struct sockaddr *) &serv_udp, sizeof(serv_udp));
	if(ret<0)
	{
		perror("Error binding udp: ");
		exit(1);
	}
	//прослушка TCP сокета
	ret = listen(sock_tcp, 5);
	if(ret<0)
	{
		perror("Error listening: ");
		exit(1);
	}
	//создание монитора событий
	epollfd=epoll_create1(0);
	if(epollfd<0)
	{
		perror("Error epoll: ");
		exit(1);
	}
	//назначение событий для просмотра
	ev_tcp.events=EPOLLIN;
	ev_tcp.data.fd=sock_tcp;
	ev_udp.events=EPOLLIN;
	ev_udp.data.fd=sock_udp;
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_tcp, &ev_tcp)<0)
	{
		perror("Error epoll_ctl add tcp: ");
		exit(1);
	}
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_udp, &ev_udp)<0)
	{
		perror("Error epoll_ctl add udp: ");
		exit(1);
	}
	while(1)
	{
		//проверка событий в TCP и UDP сокетах
		chk=epoll_wait(epollfd, events, MAX_EVENTS, 1);
		if(chk<0)
		{
			perror("Error epoll_wait: ");
			exit(1);
		}
		for(int n=0; n<chk; n++)
		{
			if(events[n].data.fd==sock_tcp)
			{
				//событие в TCP сокете
				memset(&cli, 0, sizeof(struct sockaddr_in));
				len=sizeof(struct sockaddr_in);
				client=accept(sock_tcp, (struct sockaddr *) &cli, &len);
				if(client<0)
				{
					perror("Error accept: ");
					exit(1);
				}
				//setnonblocking(client);
				ret = recv(client, &buffer, sizeof(buffer), 0);
				if(ret<0)
				{
					perror("Error read: ");
					exit(1);
				}
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
			}
			else if(events[n].data.fd==sock_udp)
			{
				//событие в UDP сокете
				memset(&cli, 0, sizeof(struct sockaddr_in));
				len=sizeof(struct sockaddr_in);
				ret=recvfrom(sock_udp, buffer, sizeof(buffer), 0, (struct sockaddr *) &cli, (unsigned int*) &len);
				if(ret<0)
				{
					perror("Error recvfrom: ");
					exit(1);
				}
				current_time = time(NULL);
				ret=sendto(sock_udp, &current_time, sizeof(current_time), MSG_CONFIRM, (struct sockaddr *) &cli, len);
				if(ret<0)
				{
					perror("Error sendto: ");
					exit(1);
				}
			}
		}
	}
}

int main()
{
	pthread_t thread;
	pthread_create(&thread, NULL, processing, NULL);
	pthread_join(thread, NULL);
	exit(0);
}