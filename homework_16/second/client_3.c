#include "af_inet_dgram.h"

void main()
{
    /* получение от сервера регистрации порта сервера-обработчика */
    /* инициализация структуры клиента */
    struct sockaddr_in serv, clnt;
	time_t current_time;
	int port;
	struct tm *date;
    /* создаем сокет */      
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd<0)
    {
        perror("client socket error");
        exit(EXIT_FAILURE);
    }
    /* заполняем структуру */
    memset(&serv, 0, sizeof(struct sockaddr_in));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(SOCK_ADDRESS);
    serv.sin_port = htons(SOCK_PORT);
    char msg[5] = {0};
    /* помещаем в буфер сообщение и отправляем его */
    strncpy(msg, "Date\n", 5);
    if (sendto(fd, msg, 5, MSG_CONFIRM, (struct sockaddr *) &serv, sizeof(struct sockaddr_in))<0)
        perror("sendto");
    socklen_t len = sizeof(struct sockaddr_in);
    if (recvfrom(fd, &current_time, sizeof(current_time), MSG_WAITALL, (struct sockaddr *)&clnt, &len)<0)
    {
        perror("recvfrom");
    }
	else
	{
		port = ntohs(clnt.sin_port);
		date=localtime(&current_time);
		printf("Server date from port %d: %d %d %d", port, date->tm_mday, date->tm_mon, date->tm_year);
	}
    sleep(1);
    close(fd);
}