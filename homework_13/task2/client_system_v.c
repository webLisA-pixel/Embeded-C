#include "message_system_v.h"

int main(void)
{
	struct mgmt_msg message_mgmt;
	struct subscriber user;
	key_t mgmt = ftok("server_system_v.c", 65);
	int mgmt_msg = msgget(mgmt, 0666);
	key_t text_snd = ftok("client_system_v.c", 65);
	user.queue_snd = msgget(text_snd, 0666);
	key_t text_rcv = ftok("server_func.c", 65);
	user.queue_rcv = msgget(text_rcv, 0666);
	char *tmp;
	char s;
	int y;
	int x;
	//Initializeing windows
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
	message_mgmt.type = 1;
	wprintw(user.win_down, "Enter your name: ");
	wrefresh(user.win_down);
	wscanw(user.win_down, "%s", message_mgmt.msg);
	//Start menegment message transmit for recive number id client on server
	tmp=strtok(message_mgmt.msg, "\n");
	user.name=strdup(tmp);
	while(tmp!=NULL)
		tmp=strtok(NULL, "\0");
	msgsnd(mgmt_msg, &message_mgmt, size_mgmt_msg, 0);
	memset(message_mgmt.msg, 0, size_mgmt_msg);
	msgrcv(mgmt_msg, &message_mgmt, sizeof(message_mgmt.msg), 0, 0);
	tmp = strtok(message_mgmt.msg, "\0");
	user.id = *tmp -'0';
	while(tmp!=NULL)
		tmp=strtok(NULL, "\0");
	s=(char)(user.id+'0');
	tmp=&s;
	//Started thread send and recive message
	pthread_create(&thread_chat_snd, NULL, chat_client_snd, (void*)&user);
	pthread_create(&thread_chat_rcv, NULL, chat_client_rcv, (void*)&user);
	pthread_join(thread_chat_snd, NULL);
	pthread_cancel(thread_chat_rcv);
	pthread_join(thread_chat_rcv, NULL);
	memset(message_mgmt.msg, 0, sizeof(message_mgmt.msg));
	strcpy(message_mgmt.msg, "exit ");
	strcat(message_mgmt.msg, tmp);
	strcat(message_mgmt.msg, "\n");
	msgsnd(mgmt_msg, &message_mgmt, size_mgmt_msg, 0);
	return 0;
}

