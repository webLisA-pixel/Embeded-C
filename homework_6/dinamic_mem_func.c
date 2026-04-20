#include "dinamic_mem_head.h"
#include <malloc.h>
#include <stdlib.h>
#include <alloca.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

int add_subscriber_first(struct abonent_list *ptr, struct inform *info){
	printf("Input subscraber name: ");	//fill struct field name
	scanf("%s", ptr->name);	//fill fild name
	printf("Input subscriber second name: ");	//fill struct field second_name
	scanf("%s", ptr->second_name);	//fill fild second_name
	printf("Input subscriber number telephon: ");	//fill struct field tel
	scanf("%s", ptr->tel);	//fill fild tel
	info->first=ptr;
	info->last=ptr;
	info->count++;
}
int add_subscriber(struct abonent_list *ptr, struct inform *info){
	struct abonent_list *ptr1;	//defination auxiliary struct
	if(info->count>100) abort();	//cheack fullness list
	while (ptr->next!=NULL) ptr=ptr->next;
	ptr->next = malloc(sizeof(*ptr));	//allocate memory by next element of struct
	if(ptr<0){	//cheack error memory
		perror("Out of memory!|n");
		exit(1);
	}
	ptr1=ptr;
	ptr=ptr->next;
	printf("Input subscraber name: ");	//fill struct field name
	scanf("%s", ptr->name);	//fill fild name
	printf("Input subscriber second name: ");	//fill struct field second_name
	scanf("%s", ptr->second_name);	//fill fild second_name
	printf("Input subscriber number telephon: ");	//fill struct field tel
	scanf("%s", ptr->tel);	//fill fild tel
	ptr->prev = ptr1;
	info->last=ptr;
	info->count++;
}

int delete_subscriber(struct inform *info){
	char search[20];
	struct abonent_list *ptr;	//defination auxiliary struct
	struct abonent_list *ptr_prev;	//defination auxiliary struct
	struct abonent_list *ptr_next;	//defination auxiliary struct
	printf("Input subscriber name for search: ");	
	scanf("%s", search);	//input subscribr name for delete
	ptr=info->first;
	while(ptr!=NULL)
	{
		if(strcmp(ptr->name, search)==0)
		{
			printf("%s\t%s\t%s\n", ptr->name, ptr->second_name, ptr->tel);	//output subscriber for delete
			if(ptr==info->first)
			{
				if(info->last==ptr)
				{
					info->first=NULL;	//zeroing pointer on first struct
					info->last=NULL;	//zeroing pointer on last struct
				}
				else info->first=ptr->next;
				free(ptr);	//deallocate memory by deleting subscriber
				info->count--;	//decline counter subcriber
				continue;
			}
			else if(ptr==info->last)
			{
				if(ptr->prev==info->first) info->last=info->first;	//edit pointer on last struct
				else info->last=ptr->prev;	//edit pointer on last struct
				ptr_prev=ptr->prev;	
				ptr_prev->next=NULL;	//zeroing pointer on last struct
				free(ptr);	//deallocate memory by deleting subscriber
				info->count--;	//decline counter subcriber
			}
			else
			{
				ptr_prev=ptr->prev;
				ptr_next=ptr->next;
				ptr_prev->next=ptr_next;
				ptr_next->prev=ptr_prev;
				free(ptr);	//deallocate memory by deleting subscriber
				info->count--;	//decline counter subcriber
			}
		}
		ptr=ptr->next;	//enumeration list
	}
}

int search_subscriber(struct inform *info){
	char search[20];
	struct abonent_list *ptr;	//defination auxiliary struct
	printf("Input subscriber name for search: ");
	scanf("%s", search);	//input subscribr name for search
	ptr=info->first;
	while(ptr!=NULL)
	{
		if(strcmp(ptr->name, search)==0)
		{
			printf("%s\t%s\t%s\n", ptr->name, ptr->second_name, ptr->tel);	//output serching subscriber
		}
		ptr=ptr->next;	//enumeration list
	}
}

int print_list(struct inform *info){
	struct abonent_list *ptr;	//defination auxiliary struct
	if(info->first==NULL) printf("Empty list!\n");	//inform about empty list
	ptr=info->first;
	while(ptr!=NULL)
	{
		printf("%s\t%s\t%s\n", ptr->name, ptr->second_name, ptr->tel);	//output subscribers
		ptr=ptr->next;	//enumeration list
	}
}

int free_mem(struct inform *info){
	struct abonent_list *ptr;	//defination auxiliary struct
	struct abonent_list *ptr_next;	//defination auxiliary struct
	ptr=info->first;
	while(ptr!=NULL)
	{
		ptr_next=ptr->next;	//defination next struct
		free(ptr);	//deallocate
		ptr=ptr_next;	//enumeration list
	}
	free(info);	//deallocate struct info
}