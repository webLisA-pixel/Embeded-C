#include <stdio.h>
#include <string.h>

struct abonent {
		char name[20];
		char second_name[20];
		char tel[15];
	};

int main(void)
{
	int number_menu=0;
	int count=1;
	char search[20]="";
	struct abonent list[100];
	struct abonent *ptr;
	ptr=&list[count];
	while(number_menu!=5)
	{
		printf("1) Add subscriber\n2) Delete subscriber\n3) Search subscriber by name\n4) Print all recors\n5) Exit\n");
		printf("Input number: ");
		scanf("%d", &number_menu);
		if(number_menu==1)
		{
			if(count>=0 && count<100)
			{
				for(int i=0;i<count;i++)
				{
					ptr=&list[i];
					if(strcmp(ptr->name, "0")==0)
					{
						printf("Input subscraber name: ");
						scanf("%s", ptr->name);
						printf("Input subscriber second name: ");
						scanf("%s", ptr->second_name);
						printf("Input subscriber number telephon: ");
						scanf("%s", ptr->tel);
						break;
					}
					if(strcmp(ptr->name, "")==0)
					{
						printf("Input subscraber name: ");
						scanf("%s", ptr->name);
						printf("Input subscriber second name: ");
						scanf("%s", ptr->second_name);
						printf("Input subscriber number telephon: ");
						scanf("%s", ptr->tel);
						count++;
						break;
					}
				}
			}
			else
			{
				printf("Number records 100, need delete one or more");
				continue;
			}
		}
		if(number_menu==2)
		{
			printf("Input subscriber name for delete: ");
			scanf("%s", search);
			for(int i=0;i<count;i++)
			{
				ptr=&list[i];
				if(strcmp(ptr->name, search)==0)
				{
					printf("Subscriber %s is delete\n", ptr->name);
					strcpy(ptr->name, "0");
					strcpy(ptr->second_name, "0");
					strcpy(ptr->tel, "0");
					count--;
					break;
				}
			}
		}
		if(number_menu==3)
		{
			printf("Input subscriber name: ");
			scanf("%s", search);
			for(int i=0;i<count;i++)
			{
				ptr=&list[i];
				if(strcmp(ptr->name, search)==0)
					printf("name: %s\tsecond_name: %s\ttelephon: %s\n",  ptr->name,  ptr->second_name,  ptr->tel);
			}
		}
		if(number_menu==4)
		{
			for(int i=0;i<count;i++)
				{
					ptr=&list[i];
					if(strcmp(ptr->name, "0")==0)
						continue;
					printf("name: %s\tsecond_name: %s\ttelephon: %s\n",  ptr->name,  ptr->second_name,  ptr->tel);
				}
		}
		if(number_menu<0 || number_menu>5)
			printf("Input incurrect nomber. Please use number 1-5!\n");
	}
}