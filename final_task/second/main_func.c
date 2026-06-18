#include "main.h"
//обработка команды введенной в CLI и разделение ее на блоки
char **parse_input(char *input)
{
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    token = strtok(input, " ");
    while (token != NULL) {
        tokens[position++] = token;
        token = strtok(NULL, " ");
    }
    tokens[position] = NULL;
    return tokens;
}
//Функция для Driver
void process()
{
	//jnrhsnbt очереди сообщений
	key_t key = ftok("main.h", 50);
	if(key<0)
	{
		perror("Error ftok:");
		exit(1);
	}
	int task = msgget(key, 0666);
	struct processing driver;
	struct task_msg task_msg;
	//заполнение структуры статуса работы driver
	driver.pid=getpid();
	driver.id = 0;
	driver.status = 0;
	driver.exit=0;
	driver.timer=0;
	task_msg.type=1;
	task_msg.driver=driver;
	//получение id driver от сервера
	if(msgsnd(task, &task_msg, sizeof(struct processing), 0)<0)
	{
		perror("Error send msg: ");
		exit(1);
	}
	msgrcv(task, &task_msg, sizeof(struct processing), 0, 0);
	driver=task_msg.driver;
	task_msg.type=driver.id;
	while(1)
	{
		//получение задачи от сервера и переход в рабочее состояние
		msgrcv(task, &task_msg, sizeof(struct processing), 0, 0);
		if(driver.pid==task_msg.driver.pid)
		{
			if(task_msg.driver.exit!=0)
				break;
			if(task_msg.driver.timer!=0)
			{
				driver=task_msg.driver;
				while(driver.timer!=0)
				{
					sleep(1);
					driver.timer--;
					printf("%d", driver.timer);
					fflush(stdout);
					if(driver.timer==0)
						driver.status=1;
					task_msg.driver=driver;
					msgsnd(task, &task_msg, sizeof(struct processing), 0);
					sleep(1);
				}
			}
			else continue;
		}
		else continue;
	}
}
//Функция для управления drivers
struct processing *process_command(char* input, struct processing* drivers, int *task)
{
	int pid, timer;
	char **args;
	struct task_msg task_msg;
	int count_drivers;
	struct processing *view_driver;
	input[strcspn(input, "\n")] = '\0';
	if (strlen(input) == 0) 
		return drivers;	
	args = parse_input(input);
	//создание нового driver
	if(strcmp(args[0], "create_driver")==0)
	{
		if(drivers==NULL)
		{
			//заполнение информационной структуры driver и запуск дочернего процесса
			drivers=malloc(sizeof(struct processing));
			drivers->id=2;
			drivers->status=1;
			drivers->timer=0;
			drivers->pid=fork();
			if(drivers->pid==0)
				process();
		}
		else
		{
			//нахождение пустого места в структуре, заполнение информационной структуры о driver и запуск дочернего процесса
			view_driver=drivers;
			count_drivers=view_driver->id;
			while(view_driver->next!=NULL)
			{
				view_driver-view_driver->next;
				count_drivers++;
			}
			view_driver->next=malloc(sizeof(struct processing));
			view_driver=view_driver->next;
			view_driver->id=count_drivers;
			view_driver->status=1;
			view_driver->timer=0;
			view_driver->pid=fork();
			if(view_driver->pid==0)
				process();
		}
	}
	//отправка задачи для driver
	else if(strcmp(args[0], "send_task")==0)
	{
		view_driver=drivers;
		sscanf(args[1], "%d", &pid);
		sscanf(args[2], "%d", &timer);
		while(view_driver!=NULL)
		{
			if(view_driver->pid==pid)
			{
				if(view_driver->status==2)
				{
					//ошибочный вывод если driver занят
					printf("Driver %d is busy by %d\n", view_driver->pid, view_driver->timer);
					break;
				}
				else if(view_driver->status==1)
				{
					//заполнение структуры и отправка задачи
					view_driver->status=2;
					view_driver->timer=timer;
					task_msg.type=view_driver->id;
					task_msg.driver=*view_driver;
					msgsnd(*task, &task_msg, sizeof(struct processing), 0);
					break;
				}
			}
			view_driver=view_driver->next;
		}
	}
	//получение статуса определенного driver
	else if(strcmp(args[0], "get_status")==0)
	{
		sscanf(args[1], "%d", &pid);
		view_driver=drivers;
		while(view_driver!=NULL)
		{
			if(view_driver->pid==pid)
			{
				if(view_driver->status==1)
				{
					printf("Driver %d, is available\n", view_driver->pid);
					break;
				}
				else
				{
					printf("Driver %d, is busy %d seconds\n", view_driver->pid, view_driver->timer);
					break;
				}
			}
			view_driver=view_driver->next;
		}
	}
	//получение статуса всех driver
	else if(strcmp(args[0], "get_drivers")==0)
	{
		view_driver=drivers;
		while(view_driver!=NULL)
		{
			if(view_driver->status==1)
			{
				printf("Driver %d is available\n", view_driver->pid);
				break;
			}
			else if(view_driver->status==2)
			{
				printf("Driver %d is busy %d seconds\n", view_driver->pid, view_driver->timer);
				break;
			}
			view_driver=view_driver->next;
		}
	}
	//выход из программы
	else if(strcmp(args[0], "exit")==0)
	{
		view_driver=drivers;
		while(view_driver!=NULL)
		{
			task_msg.type=view_driver->id;
			view_driver->exit=1;
			task_msg.driver=*view_driver;
			msgsnd(*task, &task_msg, sizeof(struct processing), 0);
			wait(NULL);
			view_driver=view_driver->next;
		}
		msgctl(*task, IPC_RMID, NULL);
		exit(0);
	}
	//информация по командам
	else if(strcmp(args[0], "?")==0)
	{
		printf("create_driver\t\t\tCreate new driver for process command\n");
		printf("send_task <pid> <task_timer>\tSend task on time processing in driver by pid\n");
		printf("get_status <pid>\t\tGet status driver by pid\n");
		printf("get_drivers\t\t\tGet status all created driver\n");
		printf("exit\t\t\t\tClose \n");
	}
	return drivers;
}