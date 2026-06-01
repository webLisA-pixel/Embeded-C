#include "message_system_v.h"

struct subscriber* sub[20];
//Function menegment message queue
void *mgmt_thread(void *args)
{
	struct mgmt_msg message_mgmt;
	key_t mgmt = ftok("server_system_v.c", 65);
	int mgmt_msg = msgget(mgmt, 0666 | IPC_CREAT);
	int count = 0;
	char *search;
	int exit_sub;
	char *tmp;
	message_mgmt.type = 1;
	while(1)
	{	//Recive message from client
		if(msgrcv(mgmt_msg, &message_mgmt, sizeof(message_mgmt.msg), 0, 0)<0)
			perror("Error recive message");
		search = strtok(message_mgmt.msg, " ");
		if(strcmp(search, "exit")==0)
		{
			search = strtok(NULL, " ");
			search = strtok(search, "\n");
			exit_sub = *search -'0';
			while(search!=NULL)
				search=strtok(NULL, "\n");
			for(int i=0; i<count; i++)
			{	//Delet client from list
				if(sub[i]->id == exit_sub)
				{
					printf("%s disconnected\n", sub[i]->name);
					memset(sub[i]->name, 0, sizeof(sub[i]->name));
					count--;
					break;
				}
			}
			if(count==0)
			{
				msgctl(mgmt_msg, IPC_RMID, NULL);
				msgctl(text_msg_rcv, IPC_RMID, NULL);
				msgctl(text_msg_snd, IPC_RMID, NULL);
				return NULL;
			}
			continue;
		}
		count++;
		for(int i=0; i<count; i++)
		{	//Add new client in list
			if(sub[i]==NULL || strcmp(sub[i]->name, "")==0)
			{
				sub[i]=malloc(sizeof(struct subscriber));
				sub[i]->id = count;
				sub[i]->new_sub=1;
				tmp=strtok(message_mgmt.msg, "\n");
				sub[i]->name=strdup(tmp);
				printf("%s connected\n", sub[i]->name);
				memset(message_mgmt.msg, 0, sizeof(message_mgmt.msg));
				memset(tmp, 0, sizeof(*tmp));
				*tmp=(char)(sub[i]->id +'0');
				strcpy(message_mgmt.msg, tmp);
				msgsnd(mgmt_msg, &message_mgmt, sizeof(message_mgmt.msg), 0);
				memset(message_mgmt.msg, 0, sizeof(message_mgmt.msg));
			}	
		}
	}
}
//Function transmit data betwin client and server
void *chat_server(void *args)
{
	struct text_msg
	{
	long type;
	char msg[size_text_msg];
	} message_text;
	char *history[100]={NULL};
	int count=0;
	int number_sub;
	int number_hist;
	int new_sub;
	char *tmp;
	//Create queue for transmit
	key_t text_rcv = ftok("client_system_v.c", 65);
	int text_msg_rcv = msgget(text_rcv, 0666 | IPC_CREAT);
	key_t text_snd = ftok("server_func.c", 65);
	int text_msg_snd = msgget(text_snd, 0666 | IPC_CREAT);
	while(1)
	{	//Add in history and retransmit message by client to client
		memset(message_text.msg, 0, sizeof(message_text.msg));
		if(msgrcv(text_msg_rcv, &message_text, sizeof(message_text.msg), 0, 0)<0)
			perror("Error recive message");
		history[count]=strdup(message_text.msg);
		number_sub=0;
		while(sub[number_sub]!=NULL)
		{
			message_text.type=sub[number_sub]->id;
			if(sub[number_sub]->new_sub==1)
			{
				number_hist=0;
				while(history[number_hist]!=NULL)
				{
					memset(message_text.msg, 0, sizeof(message_text.msg));
					strcat(message_text.msg, history[number_hist]);
					msgsnd(text_msg_snd, &message_text, sizeof(message_text.msg), 0);
					number_hist++;
				}
				sub[number_sub]->new_sub=0;
			}
			else
				msgsnd(text_msg_snd, &message_text, sizeof(message_text.msg), 0);
			number_sub++;
		}
		printf("%s", message_text.msg);
		count++;
	}
}
//Client function send message
void *chat_client_snd(void *args)
{
	struct text_msg
	{
		long type;
		char msg[size_text_msg];
	} message_text;
	char *tmp;
	struct subscriber *user = (struct subscriber*)args;
	message_text.type = user->id;
	while(1)
	{
		wclear(user->win_down);
		box(user->win_down, 0, 0);
		wmove(user->win_down, 1, 1);
		memset(message_text.msg, 0, size_text_msg);
		wprintw(user->win_down, "Enter your message: ");
		wrefresh(user->win_down);
		char msg_text[size_scan_text]="";
		wgetnstr(user->win_down, msg_text, size_scan_text);
		tmp=strtok(msg_text, "\n");
		if(strcmp(tmp, "exit")==0)
			return NULL;
		strcat(message_text.msg, user->name);
		strcat(message_text.msg, ": ");
		strcat(message_text.msg, msg_text);
		strcat(message_text.msg, "\n");
		msgsnd(user->queue_snd, &message_text, size_text_msg, 0);
	}
}
//Client function recive message and printing
void *chat_client_rcv(void *args)
{
	struct text_msg
	{
		long type;
		char msg[size_text_msg];
	} message_text;
	struct subscriber *user = (struct subscriber*)args;
	while(1)
	{
		if(msgrcv(user->queue_rcv, &message_text, sizeof(message_text.msg), user->id, 0)<0)
			perror("Error recive message");
		wprintw(user->win_up, "%s", message_text.msg);
		wrefresh(user->win_up);
	}
}