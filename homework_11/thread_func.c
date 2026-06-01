#include "thread.h"

struct common* market[5];
//Function market
void *shop(void *arg)
{
	struct common *buyer = (struct common *) arg;
	int number_market;
	while (buyer->assort>0)
	{
		number_market=rand()%5;
		pthread_mutex_lock(&market[number_market]->m_lock);
		printf("The %s wants to buy for %i came to the %s with %i\n", buyer->name, buyer->assort, market[number_market]->name, market[number_market]->assort);
		if(buyer->assort > market[number_market]->assort)
		{
			buyer->assort-=market[number_market]->assort;
			market[number_market]->assort=0;
		}
		else{
			market[number_market]->assort-=buyer->assort;
			buyer->assort=0;
			pthread_mutex_unlock(&market[number_market]->m_lock);
			pthread_exit(0);
		}		
		pthread_mutex_unlock(&market[number_market]->m_lock);
		sleep(2);
	}
	return NULL;
}
//Function supplier
void *supplier(void *args)
{
	int number_market;
	while (1)
	{
		number_market=rand()%5;
		pthread_mutex_lock(&market[number_market]->m_lock);
		printf("The supplier came to the %s and add 2000\n", market[number_market]->name);
		market[number_market]->assort+=200;
		pthread_mutex_unlock(&market[number_market]->m_lock);
		sleep(1);
	}
	return NULL;
}
