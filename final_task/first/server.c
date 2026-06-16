#include "raw.h"

int main()
{
	pthread_t thread;
	pthread_create(&thread, NULL, server_control, NULL);
	pthread_join(thread, NULL);
	exit(0);
}