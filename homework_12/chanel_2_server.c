#include "chanel_2.h"

int main(void)
{
	char *string="Hi!";
	int pipe_write;
	char *path="/home/alexandr/buff";
	mkfifo(path, 0666);
	pipe_write=open(path, O_WRONLY);
	write(pipe_write, string, strlen(string)+1);
	close(pipe_write);
	exit(0);
}