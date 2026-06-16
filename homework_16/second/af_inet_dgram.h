#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h> 
#include <time.h>

#define SOCK_ADDRESS "127.0.0.1"
#define SOCK_PORT 48888