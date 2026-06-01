#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

int main()
{
	int pid;
	printf("Enter PID: ");
	scanf("%d", &pid);
	kill(pid, SIGUSR1);
	exit(0);
}