#include "message_system_v.h"

int main(void)
{
	key_t key = ftok("buffer.txt", 50);
	int chat = msgget(key, 0666);
	message.type = 1;
	while(strcmp(message.msg, "")==0)
		msgrcv(chat, &message, sizeof(message.msg), 0, 0);
	printf("Recive message: %s\n", message.msg);
	strcpy(message.msg, "Hello!");
	msgsnd(chat, &message, sizeof(message.msg), 0);
	return 0;
}