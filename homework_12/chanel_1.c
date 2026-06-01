#include "chanel_1.h"

int main(void)
{
	char* string1="Hi!";
	char string2[length];
	pid_t pid;
	int pipe1[2];
	int status;
	if(pipe(pipe1)<0)
	{
		perror("Error pipe!");
		exit(1);
	}
	switch(fork())
	{
		case -1:
			perror("Can't create process!");
			exit(1);
		case 0:
			close(pipe1[1]);
			while(1)
			{
				read(pipe1[0], string2, length);
				if(string2!="")
					break;
			}
			printf("%s", string2);
			exit(0);
		default:
			close(pipe1[0]);
			sleep(3);
			write(pipe1[1], string1, length);
			wait(&status);
	}
	exit(0);
}