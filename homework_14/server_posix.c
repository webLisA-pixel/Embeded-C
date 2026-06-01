#include "message_posix.h"

int main(void)
{
	int shm_fd;
	shm_fd = shm_open("/message_posix.c", O_CREAT | O_RDWR, 0660);
	if(shm_fd < 0)
	{
		perror("shm_open: ");
		exit(1);
	}
	if(ftruncate(shm_fd, size_msg)<0)
	{
		perror("ftruncate: ");
		exit(1);
	}
	char *message = mmap(NULL, size_msg, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(message < 0)
	{
		perror("mmap: ");
		close(shm_fd);
		exit(1);
	}
	close(shm_fd);
	strncpy(message, "Hi!", size_msg);
	sleep(2);
	printf("Receive message %s\n", message);
	if(munmap(message, size_msg))
	{
		perror("munmap: ");
		exit(1);
	}
	if(shm_unlink("/message_posix.c")<0)
	{
		perror("shm_unlink: ");
		exit(1);
	}
	return 0;
}