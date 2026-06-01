#include "message_system_v.h"

extern struct subscriber* sub[20];

int main(void)
{	//start threads for server
	pthread_t thread_mgmt;
	pthread_t thread_chat;
	pthread_create(&thread_mgmt, NULL, mgmt_thread, NULL);
	pthread_create(&thread_chat, NULL, chat_server, NULL);
	pthread_join(thread_mgmt, NULL);
	pthread_join(thread_chat, NULL);
	return 0;
}