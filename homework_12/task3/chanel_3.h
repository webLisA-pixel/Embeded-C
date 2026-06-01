#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <malloc.h>

#define BUFFER_SIZE 1024
#define HISTORY_SIZE 10
#define bufsize 64
#define READ 0
#define WRITE 1

static char *history[HISTORY_SIZE];
static int history_count = 0;

void add_history(char *command);
void print_history();
char *read_input();
char **parse_input(char *input);
int cd(char **args);
void execute(char **args);