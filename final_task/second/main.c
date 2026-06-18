#include "main.h"

int main()
{
	struct epoll_event ev_input, events[MAX_EVENTS];
	int epollfd, check;
	char *input = malloc(BUFFER_SIZE * sizeof(char));;
	struct processing *drivers=NULL;
	struct processing *view_driver;
	//Открытие очереди сообщений
	struct task_msg task_msg;
	int size_msg=sizeof(struct processing);
	key_t key = ftok("main.h", 50);
	if(key<0)
	{
		perror("Error ftok:");
		exit(1);
	}
	int task = msgget(key, 0666 | IPC_CREAT);
	epollfd=epoll_create1(0);
	if(epollfd<0)
	{
		perror("Error epoll create: ");
		exit(1);
	}
	task_msg.type=0;
	//Создание мониторинга событий
	ev_input.events=EPOLLIN;
	ev_input.data.fd=STDIN_FILENO;
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev_input)<0)
	{
		perror("Error epoll_ctl: ");
		exit(1);
	}
	// Display the prompt
	printf("%s@:> ", getenv("USER"));
	fflush(stdout);
	while (1)
	{
		//проверка события ввода команды в CLI и ее обработка
		check=epoll_wait(epollfd, events, MAX_EVENTS, 100);
		if(check<0)
		{
			perror("Error epoll wait: ");
			exit(1);
		}
		for(int i=0;i<check;i++)
		{
			if(events[i].data.fd==STDIN_FILENO)
				// Read input
				fgets(input, BUFFER_SIZE, stdin);
				drivers=process_command(input, drivers, &task);
				printf("%s@:> ", getenv("USER"));
				fflush(stdout);
		}
		//Примем сообщений из очереди от driver
		msgrcv(task, &task_msg, sizeof(task_msg.driver), 0, IPC_NOWAIT);
		if(task_msg.driver.id==1)
		{
			view_driver=drivers;
			while(view_driver!=NULL)
			{
				if(view_driver->pid==task_msg.driver.pid)
				{
					task_msg.driver=*view_driver;
					msgsnd(task, &task_msg, sizeof(struct processing), 0);
					break;
				}
				view_driver=view_driver->next;
			}
		}
		//обновление информации о статусе driver
		else
		{
			view_driver=drivers;
			while(view_driver!=NULL)
			{
				if(view_driver->pid==task_msg.driver.pid)
				{
					*view_driver=task_msg.driver;
					break;
				}
				view_driver=view_driver->next;
			}
		}
	}
}