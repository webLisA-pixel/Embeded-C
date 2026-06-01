#include "message_posix.h"

int main(void)
{
	mqd_t msg_queue;
	char *msg_rcv=malloc(size_msg*sizeof(char));
	msg_queue = mq_open("/buffer.txt", O_RDWR);
	if(msg_queue==(mqd_t)-1)
	{
		perror("Error queue open");
		exit(1);
	}
	if(mq_receive(msg_queue, msg_rcv, size_msg, 0)<0)
		perror("Error receive message"), exit(1);
	printf("Receive message%s", msg_rcv);
	if(mq_send(msg_queue, "Hello!", size_msg, 0)<0)
		perror("Error send message"), exit(1);
	mq_close(msg_queue);
	free(msg_rcv);
	return 0;
}