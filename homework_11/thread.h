#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

#define n 3

struct common
{
	pthread_mutex_t m_lock;
	char *name;
	int assort;
};

void *shop(void *args);
void *supplier(void *args);