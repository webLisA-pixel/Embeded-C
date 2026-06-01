#include "message_system_v.h"

int main(void)
{
	key_t key = ftok("buffer.txt", 50);
	int chat = msgget(key, 0666 | IPC_CREAT);
	message.type = 1;
	strcpy(message.msg, "Hi!");
	msgsnd(chat, &message, sizeof(message.msg), 0);
	memset(message.msg, 0, sizeof(message.msg));
	sleep(2);
	if(msgrcv(chat, &message, sizeof(message.msg), 0, 0)<0)
		perror("Error recive message");
	printf("Recive message: %s\n", message.msg);
	msgctl(chat, IPC_RMID, NULL);
	return 0;
}