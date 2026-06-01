#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

int main()
{
	sigset_t set;
	int ret;
	int sig_num;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	ret = sigprocmask(SIG_BLOCK, &set, NULL);
	if(ret<0)
	{
		perror("Sig mask: ");
		exit(1);
	}
	while(1)
	{
		sigwait(&set, &sig_num);
		printf("SIG number: %d\n", sig_num);
	}
	exit(0);
}