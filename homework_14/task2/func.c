#include "message_system_v.h"

struct subscriber* sub[20];
int mgmt_exit;
//Function server managment control
void *mgmt_thread(void *args)
{
	
	struct msg message_mgmt;
	struct sembuf LOCK = {0, -1, 0};
	struct sembuf UNLOCK = {0, 1, 0};
	union semun sem_union;
	int sem_value;
	sem_union.val=1;
	//Initializing shared memory and semaphore
	key_t mgmt_key = ftok("message_system_v.h", 65);
	if(mgmt_key<0)
	{
		perror("ftok mgmt server: ");
		exit(1);
	}
	int mgmt = shmget(mgmt_key, size_sub_name, 0666 | IPC_CREAT);
	if(mgmt<0)
	{
		perror("shmget mgmt  server: ");
		exit(1);
	}
	struct msg *mgmt_msg = (struct msg *)shmat(mgmt, NULL, 0);
	if(mgmt_msg<0)
	{
		perror("shmat mgmt  server: ");
		exit(1);
	}
	int sem_mgmt = semget(mgmt_key, 1, 0666 | IPC_CREAT);
	if(sem_mgmt<0)
	{
		perror("semget mgmt  server: ");
		exit(1);
	}
	if(semctl(sem_mgmt, 0, SETVAL, sem_union))
	{
		perror("semctl mgmt  server: ");
		exit(1);
	}
	int count_sub;
	while(1)
	{
		if(mgmt_msg->id_sub==0)
		{
			if(strcmp(mgmt_msg->msg, "")!=0)
			{
				count_sub++;
				for(int i=0; i<count_sub; i++)
				{	//Add Client to list
					if(sub[i]==NULL)
					{
						sub[i]=malloc(sizeof(struct subscriber));
						semop(sem_mgmt, &LOCK, 1);
						sub[i]->id=count_sub;
						sub[i]->name=strdup(mgmt_msg->msg);
						printf("%s connected\n", sub[i]->name);
						mgmt_msg->id_sub=sub[i]->id;
						sub[i]->new_sub=1;
						break;
					}
					else if(strcmp(sub[i]->name, "")==0)
					{
						semop(sem_mgmt, &LOCK, 1);
						sub[i]->name=strdup(mgmt_msg->msg);
						printf("%s connected", sub[i]->name);
						mgmt_msg->id_sub=sub[i]->id;
						sub[i]->new_sub=1;
						break;
					}
				}
				memset(mgmt_msg->msg, 0, sizeof(mgmt_msg->msg));
				semop(sem_mgmt, &UNLOCK, 1);
			}
		}
		else if(mgmt_msg->id_sub>0)
		{
			if(strcmp(mgmt_msg->msg, "exit")==0)
			{
				for(int i=0; i<count_sub; i++)
				{	//Delet client from list
					if(mgmt_msg->id_sub=sub[i]->id)
					{
						printf("%s disconnected", sub[i]->name);
						semop(sem_mgmt, &LOCK, 1);
						memset(sub[i]->name, 0, sizeof(sub[i]->name));
						break;
					}
				}
				count_sub--;
				mgmt_msg->id_sub=0;
				memset(mgmt_msg->msg, 0, sizeof(mgmt_msg->msg));
				semop(sem_mgmt, &UNLOCK, 1);
				if(count_sub==0) 
					break;
			}
		}
	}
	//Unlink shared memory
	if(shmdt(mgmt_msg)<0)
	{
		perror("shmdt mgmt server: ");
		exit(1);
	}
	if(shmctl(mgmt, IPC_RMID, NULL)<0)
	{
		perror("shmctl mgmt server: ");
		exit(1);
	}
	if(semctl(sem_mgmt, 0, IPC_RMID, NULL))
	{
		perror("semctl mgmt  server:");
		exit(1);
	}
	mgmt_exit=1;
	return NULL;
}
//Function server transmit messages
void *chat_server(void *args)
{
	char *history[100]={NULL};
	struct sembuf LOCK = {0, -1, 0};
	struct sembuf UNLOCK = {0, 1, 0};
	struct msg *msg_snd;
	struct msg *msg_rcv;
	int count=0;
	int number_sub;
	int number_hist;
	int new_sub;
	char *tmp;
	int tmp_n;
	union semun sem_union;
	sem_union.val=1;
	//Initializing shared memory and semaphore
	key_t rcv_key = ftok("server_system_v.c", 65);
	if(rcv_key<0)
	{
		perror("ftok rcv server: ");
		exit(1);
	}
	key_t snd_key = ftok("client_system_v.c", 60);
	if(snd_key<0)
	{
		perror("ftok snd server: ");
		exit(1);
	}
	int rcv = shmget(rcv_key, size_page, 0666 | IPC_CREAT);
	if(rcv<0)
	{
		perror("shmget rcv server: ");
		exit(1);
	}
	int snd = shmget(snd_key, size_page, 0666 | IPC_CREAT);
	if(snd<0)
	{
		perror("shmget snd server: ");
		exit(1);
	}
	msg_rcv = (struct msg*)shmat(rcv, NULL, 0);
	if(msg_rcv<0)
	{
		perror("shmat rcv server: ");
		exit(1);
	}
	msg_snd = (struct msg*)shmat(snd, NULL, 0);
	if(msg_snd<0)
	{
		perror("shmat snd server: ");
		exit(1);
	}
	int sem_snd = semget(snd_key, 1, 0666 | IPC_CREAT);
	if(sem_snd<0)
	{
		perror("semget snd server: ");
		exit(1);
	}
	int sem_rcv = semget(rcv_key, 1, 0666 | IPC_CREAT);
	if(sem_snd<0)
	{
		perror("semget snd server: ");
		exit(1);
	}
	if(semctl(sem_snd, 0, SETVAL, sem_union)<0)
	{
		perror("semctl rcv server: ");
		exit(1);
	}
	if(semctl(sem_rcv, 0, SETVAL, sem_union)<0)
	{
		perror("semctl snd server: ");
		exit(1);
	}
	//recive and send message by client to client and add message to history
	while(1)
	{
		if(mgmt_exit==1)
			break;
		if(msg_rcv->id_sub==0) 
			continue;
		else if(msg_rcv->id_sub>0)
		{
			semop(sem_rcv, &LOCK, 1);
			history[count]=malloc(size_page);
			history[count]=strdup(msg_rcv->msg);
			number_sub=0;
			while(sub[number_sub]!=NULL)
			{
				if(sub[number_sub]->new_sub==1)
				{
					number_hist=0;
					while(history[number_hist]!=NULL)
					{
						if(strcmp(msg_snd->msg, "")!=0)
							continue;
						semop(sem_snd, &LOCK, 1);
						msg_snd->id_sub=sub[number_sub]->id;
						strcat(msg_snd->msg, history[number_hist]);
						number_hist++;
						semop(sem_snd, &UNLOCK, 1);
					}
					sub[number_sub]->new_sub=0;
				}
				else
				{
					if(strcmp(msg_snd->msg, "")!=0)
						continue;
					semop(sem_snd, &LOCK, 1);
					msg_snd->id_sub=sub[number_sub]->id;
					strcpy(msg_snd->msg, msg_rcv->msg);
					semop(sem_snd, &UNLOCK, 1);
				}
				number_sub++;
			}
			printf("%s", msg_rcv->msg);
			memset(msg_rcv->msg, 0, sizeof(msg_rcv->msg));
			msg_rcv->id_sub=0;
			semop(sem_rcv, &UNLOCK, 1);
			count++;
		}
	}
	//Unlink shared memory
	if(shmdt(msg_snd)<0)
	{
		perror("shmdt snd server: ");
		exit(1);
	}
	if(shmdt(msg_rcv)<0)
	{
		perror("shmdt rcv server: ");
		exit(1);
	}
	if(shmctl(snd, IPC_RMID, NULL)<0)
	{
		perror("shmctl snd server: ");
		exit(1);
	}
	if(shmctl(rcv, IPC_RMID, NULL)<0)
	{
		perror("shmctl rcv server: ");
		exit(1);
	}
	if(semctl(sem_snd, 0, IPC_RMID, NULL))
	{
		perror("semctl snd server:");
		exit(1);
	}
	if(semctl(sem_rcv, 0, IPC_RMID, NULL))
	{
		perror("semctl rcv server:");
		exit(1);
	}
	return NULL;
}
//Function client send messages
void *chat_client_snd(void *args)
{
	char *tmp;
	union semun sem_union;
	sem_union.val=1;
	struct subscriber *user = (struct subscriber*)args;
	struct sembuf LOCK = {0, -1, 0};
	struct sembuf UNLOCK = {0, 1, 0};
	key_t snd_key = ftok("server_system_v.c", 65);
	int snd = shmget(snd_key, size_page, 0666);
	struct msg *msg_snd = (struct msg *)shmat(snd, NULL, 0);
	int sem_snd = semget(snd_key, 1, 0666);
	if(semctl(sem_snd, 0, SETVAL, sem_union))
	{
		perror("semctl: ");
		exit(1);
	}
	while(1)
	{
		wclear(user->win_down);
		box(user->win_down, 0, 0);
		wmove(user->win_down, 1, 1);
		wprintw(user->win_down, "Enter your message: ");
		wrefresh(user->win_down);
		char msg_text[size_scan_text]="";
		wgetnstr(user->win_down, msg_text, size_scan_text);
		if(strcmp(msg_text, "\n")==0)
			continue;
		if(strcmp(msg_text, "exit")==0)
			break;
		semop(sem_snd, &LOCK, 1);
		msg_snd->id_sub=user->id;
		strcat(msg_snd->msg, user->name);
		strcat(msg_snd->msg, ": ");
		strcat(msg_snd->msg, msg_text);
		strcat(msg_snd->msg, "\n");
		semop(sem_snd, &UNLOCK, 1);
	}
	if(shmdt(msg_snd)<0)
	{
		perror("shmdt snd client: ");
		exit(1);
	}
	if(shmctl(snd, IPC_RMID, NULL)<0)
	{
		perror("shmctl snd client: ");
		exit(1);
	}
	if(semctl(sem_snd, 0, IPC_RMID, NULL))
	{
		perror("semctl snd client:");
		exit(1);
	}
	user->exit=1;
	return NULL;
}
//Function client recive messages and printing
void *chat_client_rcv(void *args)
{
	union semun sem_union;
	sem_union.val=1;
	struct subscriber *user = (struct subscriber*)args;
	struct sembuf LOCK = {0, -1, 0};
	struct sembuf UNLOCK = {0, 1, 0};
	key_t rcv_key = ftok("client_system_v.c", 60);
	int rcv = shmget(rcv_key, size_page, 0666);
	struct msg *msg_rcv = (struct msg *)shmat(rcv, NULL, 0);
	int sem_rcv = semget(rcv_key, 1, 0666);
	if(semctl(sem_rcv, 0, SETVAL, sem_union))
	{
		perror("semctl: ");
		exit(1);
	}
	while(1)
	{
		if(user->exit==1)
			break;
		if(msg_rcv->id_sub==user->id)
		{
			semop(sem_rcv, &LOCK, 1);
			wprintw(user->win_up, "%s", msg_rcv->msg);
			wrefresh(user->win_up);
			memset(msg_rcv->msg, 0, sizeof(msg_rcv->msg));
			msg_rcv->id_sub=0;
			semop(sem_rcv, &UNLOCK, 1);
		}
	}
	if(shmdt(msg_rcv)<0)
	{
		perror("shmdt rcv client: ");
		exit(1);
	}
	if(shmctl(rcv, IPC_RMID, NULL)<0)
	{
		perror("shmctl rcv client: ");
		exit(1);
	}
	if(semctl(sem_rcv, 0, IPC_RMID, NULL))
	{
		perror("semctl rcv client:");
		exit(1);
	}
	return NULL;
}