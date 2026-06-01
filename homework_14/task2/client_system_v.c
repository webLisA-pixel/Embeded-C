#include "message_system_v.h"

int main(void)
{
	struct msg message_mgmt;
	struct subscriber user;
	struct sembuf LOCK = {0, -1, SEM_UNDO};
	struct sembuf UNLOCK = {0, 1, SEM_UNDO};
	union semun sem_union;
	//Inicializeing shared memory and semaphore
	key_t mgmt_key = ftok("message_system_v.h", 65);
	if(mgmt_key<0)
	{
		perror("ftok: ");
		exit(1);
	}
	int mgmt = shmget(mgmt_key, size_sub_name, 0666);
	if(mgmt<0)
	{
		perror("shmget: ");
		exit(1);
	}
	struct msg *mgmt_msg = (struct msg *)shmat(mgmt, NULL, 0);
	if(mgmt_msg<0)
	{
		perror("shmat: ");
		exit(1);
	}
	int sem_mgmt = semget(mgmt_key, 1, 0666);
	if(sem_mgmt<0)
	{
		perror("semget: ");
		exit(1);
	}
	sem_union.val=1;
	if(semctl(sem_mgmt, 0, SETVAL, sem_union))
	{
		perror("semctl: ");
		exit(1);
	}
	char *tmp;
	char s;
	int y;
	int x;
	//Initializeing Windows
	initscr();
	cbreak();
	echo();
	getmaxyx(stdscr, y, x);
	user.win_up=newwin(y*0.9, x, 0, 0);
	if(user.win_down<0)
	{
		perror("Don't create new window!|n");
		exit(1);
	}
	user.win_down=newwin(y*0.1, x, y*0.9, 0);
	if(user.win_up<0)
	{
		perror("Don't create new window!|n");
		exit(1);
	}
	refresh();
	wrefresh(user.win_up);
	keypad(stdscr, true);
	pthread_t thread_chat_snd;
	pthread_t thread_chat_rcv;
	wprintw(user.win_down, "Enter your name: ");
	wrefresh(user.win_down);
	wscanw(user.win_down, "%s", tmp);
	//Start transmit managment message and recive client id by server
	user.name=strdup(tmp);
	if(semop(sem_mgmt, &LOCK, 1)<0)
	{
		perror("semop: ");
		exit(1);
	}
	strcpy(mgmt_msg->msg, user.name);
	mgmt_msg->id_sub=0;
	if(semop(sem_mgmt, &UNLOCK, 1)<0)
	{
		perror("semop: ");
		exit(1);
	}
	while(mgmt_msg->id_sub<1)
		continue;
	semop(sem_mgmt, &LOCK, 1);
	user.id = mgmt_msg->id_sub;
	mgmt_msg->id_sub=0;
	semop(sem_mgmt, &UNLOCK, 1);
	user.exit=0;
	//Start threads send and recive message
	pthread_create(&thread_chat_snd, NULL, chat_client_snd, (void*)&user);
	pthread_create(&thread_chat_rcv, NULL, chat_client_rcv, (void*)&user);
	pthread_join(thread_chat_snd, NULL);
	pthread_join(thread_chat_rcv, NULL);
	semop(sem_mgmt, &LOCK, 1);
	strcpy(mgmt_msg->msg, "exit");
	mgmt_msg->id_sub=user.id;
	semop(sem_mgmt, &UNLOCK, 1);
	if(shmdt(mgmt_msg)<0)
	{
		perror("shmdt: ");
		exit(1);
	}
	return 0;
}

