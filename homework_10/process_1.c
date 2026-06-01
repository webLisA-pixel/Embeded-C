#include "process_1.h"

int main(void)
{
	pid_t chaild_pid;
	int status;
	chaild_pid=fork();
	if (chaild_pid ==0)
	{
		printf("chaild pid: %d\t chaild ppid: %d\n", getpid(), getppid());
		exit(0);
	}
	else
	{
		printf("parent pid: %d\t parent ppid: %d\n", getpid(), getppid());
		wait(&status);
		printf("status exit chaild process: %d\n", WEXITSTATUS(status));
	}
}