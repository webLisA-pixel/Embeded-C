#include "thread.h"
//definition global variables
extern struct common *market[5];
struct common *buyer[3];

int main(void)
{
	int *s;
	pthread_t thread[n];
	char *buff_str;
	for (int i=0; i<5; i++)
		market[i]=malloc(sizeof(struct common));
	//Definition marcet choice
	buff_str="first_market";
	market[0]->name=strdup(buff_str);
	market[0]->assort=900+rand()%200;
	buff_str="second_market";
	market[1]->name=strdup(buff_str);
	market[1]->assort=900+rand()%200;
	buff_str="third_market";
	market[2]->name=strdup(buff_str);
	market[2]->assort=900+rand()%200;
	buff_str="fourth_market";
	market[3]->name=strdup(buff_str);
	market[3]->assort=900+rand()%200;
	buff_str="fifth_market";
	market[4]->name=strdup(buff_str);
	market[4]->assort=900+rand()%200;
	for (int i=0; i<3; i++)
		buyer[i]=malloc(sizeof(struct common));
	//Definition buyer choice
	buff_str="first_buyer";
	buyer[0]->name=strdup(buff_str);
	buyer[0]->assort=9900+rand()%200;
	buff_str="second_buyer";
	buyer[1]->name=strdup(buff_str);
	buyer[1]->assort=9900+rand()%200;
	buff_str="third_buyer";
	buyer[2]->name=strdup(buff_str);
	buyer[2]->assort=9900+rand()%200;
	//Start thread market
	for(int i=0; i<3; i++)	
		pthread_create(&thread[i], NULL, shop, (void *)buyer[i]);
	//Start suppluyer
	pthread_create(&thread[n], NULL, supplier, NULL);
	//waiting for the end of purchases 
	for(int j=0;j<3;j++)
	{
		pthread_join(thread[j], (void **)&s);
		printf("Buyer %s have a %i\n", buyer[j]->name, buyer[j]->assort);
	}
	//Cunsel suppluyer
	pthread_cancel(thread[n]);
	pthread_join(thread[n], (void **)&s);
	for (int i=0; i<5; i++)
		free(market[i]);
	for (int i=0; i<3; i++)
		free(buyer[i]);
	exit(0);
}