#include "process_2.h"

int main(void)
{
	pid_t process_1;
	pid_t process_2;
	pid_t process_3;
	pid_t process_4;
	pid_t process_5;
	int status_1;
	int status_2;
	int status_3;
	int status_4;
	int status_5;
	printf("parent pid: %d\t parent ppid: %d\n", getpid(), getppid());
	process_1=fork();

	if (process_1 ==0)
	{
		printf("process_1 pid: %d\t process_1 ppid: %d\n", getpid(), getppid());
		process_3 = fork();

		if (process_3 ==0)
		{
			printf("process_3 pid: %d\t process_3 ppid: %d\n", getpid(), getppid());
			exit(13);			
		}
		process_4 = fork();

		if (process_4==0)
		{
			printf("process_4 pid: %d\t process_4 ppid: %d\n", getpid(), getppid());
			exit(14);
		}
		wait(&status_3);
		wait(&status_4);
		printf("status exit process_3 process: %d\n", WEXITSTATUS(status_3));
		printf("status exit process_4 process: %d\n", WEXITSTATUS(status_4));
		exit(11);
	}

	process_2=fork();

	if (process_2==0)
	{
		printf("process_2 pid: %d\t process_2 ppid: %d\n", getpid(), getppid());
		process_5 = fork();
		
		if(process_5==0)
		{
			printf("process_5 pid: %d\t process_5 ppid: %d\n", getpid(), getppid());
			exit(15);
		}
		else
		{
			wait(&status_5);
			printf("status exit process_5 process: %d\n", WEXITSTATUS(status_5));
			exit(12);
		}
	}

	else
	{
		wait(&status_1);
		wait(&status_2);
		printf("status exit process_1 process: %d\n", WEXITSTATUS(status_1));
		printf("status exit process_2 process: %d\n", WEXITSTATUS(status_2));
	}
	exit(0);
}