#include "chanel_2.h"

int main(void)
{
	char *string;
	int pipe_read;
	char *path="/home/alexandr/buff";
	pipe_read=open(path, O_RDONLY);
	read(pipe_read, string, 4);
	printf("%s\n", string);
	close(pipe_read);
	exit(0);
}