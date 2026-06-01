#ifndef __MESSAGE_SYSTEM_V_H__
#define __MESSAGE_SYSTEM_V_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>

#define size_page 1090
#define size_msg 1024
#define size_scan_text 978
#define size_sub_name 15

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

struct subscriber
{
	int id;
	char *name;
	int new_sub;
	int exit;
	WINDOW *win_up;
	WINDOW *win_down;
};

struct msg
{
	int id_sub;
	char msg[size_msg];
};

void *mgmt_thread(void *args);
void *chat_server(void *args);
void *chat_client_snd(void *args);
void *chat_client_rcv(void *args);

#endif