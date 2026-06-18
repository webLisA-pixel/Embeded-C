#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <malloc.h>
#include <sys/epoll.h>
#include <mqueue.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFFER_SIZE 1024
#define bufsize 64
#define PATH "/main.h"
#define MAX_EVENTS 10

struct processing
{
	pid_t pid;
	int id;			//1 - New
	int status;		//0 - New 1 - Available, 2 - Buzy
	int timer;
	int exit;		//0 - work, 1 - end process
	struct processing *next;
};

struct task_msg
{
	long type;
	struct processing driver;
};

char **parse_input(char *input);
struct processing *process_command(char* input, struct processing* drivers, int *task);
void process();