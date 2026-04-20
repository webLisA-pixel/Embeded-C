#include "dinamic_mem_head.h"
#include <malloc.h>
#include <stdlib.h>
#include <alloca.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
	
int main(void)
{
	int number_menu=0;	//selected point in menu
	struct abonent_list *ptr=malloc(sizeof(*ptr)); //allocate memory by struct
	if(ptr<0){	//cheack error memory
		perror("Out of memory!|n");
		exit(1);
	}
	struct inform *info=malloc(sizeof(*info));	//allocate memory by info struct
	if(info<0){	//cheack error memory
		perror("Out of memory!|n");
		exit(1);
	}
	ptr->prev=NULL;	//definition previous element
	ptr->next=NULL;	//defination next element
	info->first=NULL;	//defination first element struct
	info->last=NULL;	//defination last element struct
	info->count=0;	//defination counter elements
	while(number_menu!=5)
	{
		printf("1) Add subscriber\n2) Delete subscriber\n3) Search subscriber by name\n4) Print all recors\n5) Exit\n");	//output menu
		printf("Input number: ");	
		scanf("%d", &number_menu);	//input	point from menu
		if(number_menu==1)
		{
			if(info->last == NULL)
			{
				add_subscriber_first(ptr, info);	//add firs element struct
			}
			else
			{
				add_subscriber(ptr, info);	//add element struct
			}
		}
		if(number_menu==2) delete_subscriber(info);	//delete element struct
		if(number_menu==3) search_subscriber(info);	//search subscriber by name
		if(number_menu==4) print_list(info);	//print all elements struct
		if(number_menu<0 || number_menu>5) printf("Input incurrect nomber. Please use number 1-5!\n");	///inform about incorrect point
	}
	free_mem(info);	//deallocate memory by all element struct
}