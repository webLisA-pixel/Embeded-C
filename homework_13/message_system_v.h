#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define size_msg 10

struct mesg_buffer
{
	long type;
	char msg[size_msg];
} message;