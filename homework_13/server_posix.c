#include "message_posix.h"

int main(void)
{
	struct mq_attr queue;
	mqd_t msg_queue;
	char *msg_rcv=malloc(size_msg*sizeof(char));
	queue = malloc(sizeof(struct mq_attr));
	if(queue==NULL)
	{
		perror("Error malloc");
		exit(1);
	}
	queue->mq_maxmsg = 10;
	queue->mq_msgsize = size_msg;
	msg_queue = mq_open("/server_posix.c", O_CREAT | O_RDWR, 0660, queue);
	if(msg_queue == (mqd_t) -1)
	{
		perror("Error queue open");
		free(queue);
		exit(1);
	}
	if(mq_send(msg_queue, "Hi!", size_msg, 0)<0)
	{
		perror("Error send message");
		exit(1);
	}
	sleep(5);
	if(mq_receive(msg_queue, msg_rcv, size_msg, 0)<0)
	{
		perror("Error receive message");
		exit(1);
	}
	printf("Receive message%s", msg_rcv);
	mq_close(msg_queue);
	mq_unlink("/home/a123/embeded/message/buffer.txt");
	free(msg_rcv);
	free(queue);
	return 0;
}