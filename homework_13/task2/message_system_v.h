#ifndef __MESSAGE_SYSTEM_V_H__
#define __MESSAGE_SYSTEM_V_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <malloc.h>
#include <pthread.h>
#include <ncurses.h>

#define size_mgmt_msg 10
#define size_text_msg 256
#define size_scan_text 200

struct subscriber
{
	int id;
	char* name;
	int new_sub;
	int queue_rcv;
	int queue_snd;
	WINDOW *win_up;
	WINDOW *win_down;
};

struct mgmt_msg
{
	long type;
	char msg[size_mgmt_msg];
};

void *mgmt_thread(void *args);
void *chat_server(void *args);
void *chat_client_snd(void *args);
void *chat_client_rcv(void *args);

#endif