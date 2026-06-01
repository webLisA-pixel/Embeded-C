#include "message_system_v.h"

int main(void)
{
	key_t key = ftok("message_system_v.h", 50);
	int chat = shmget(key, size_msg, 0666);
	if(chat<0)
	{
		perror("shmget: ");
		exit(1);
	}
	char *shm_ptr = (char *)shmat(chat, NULL, 0);
	if(shm_ptr<0)
	{
		perror("shmat: ");
		exit(1);
	}
	printf("Recive message: %s\n", shm_ptr);
	strncpy(shm_ptr, "Hello!", size_msg);
	if(shmdt(shm_ptr)<0)
	{
		perror("shmdt: ");
		exit(1);
	}
	return 0;
}