#include "message_system_v.h"

int main(void)
{
	key_t key = ftok("message_system_v.h", 50);
	int chat = shmget(key, size_msg, 0666 | IPC_CREAT);
	if(chat<0)
	{
		perror("shmget: ");
		exit(1);
	}
	char *shm_ptr=shmat(chat, NULL, 0);
	if(shm_ptr<0)
	{
		perror("shmat: ");
		exit(1);
	}
	memset(shm_ptr, 0 , size_msg);
	strncpy(shm_ptr, "Hi!", size_msg);
	sleep(2);
	printf("Recive message: %s\n", shm_ptr);
	if(shmdt(shm_ptr)<0)
	{
		perror("shmdt: ");
		exit(1);
	}
	if(shmctl(chat, IPC_RMID, NULL)<0)
	{
		perror("shmctl: ");
		exit(1);
	}
	return 0;
}