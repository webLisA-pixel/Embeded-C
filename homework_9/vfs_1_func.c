#include "vfs_1.h"

int input(int file)
{
	write(file, "String from file", strlen("String from file\n"));
}
int output(int file)
{
	char buffer[1];
	int err;
	int size_file;
	int i=0;
	size_file=lseek(file, 0, SEEK_END);
	if(size_file<0)
	{
		perror("Out of memory!\n");
		exit(1);
	}
	while(size_file>=0)
	{
		size_file--;
		lseek(file, size_file, SEEK_SET);
		err=read(file, buffer, 1);
		if(err<0)
		{
			perror("Out of memory!\n");
			exit(1);
		}
		printf("%s", buffer);
	}
}