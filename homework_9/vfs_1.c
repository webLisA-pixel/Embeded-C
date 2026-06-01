#include "vfs_1.h"

void main()
{
	int file;
	const char * name="output.txt";
	open(name, O_CREAT, O_RDWR);
	file=open(name, O_RDWR);
	if(file<0)
	{
		perror("Out of memory!\n");
		exit(1);
	}
	input(file);
	output(file);
	close(file);
}