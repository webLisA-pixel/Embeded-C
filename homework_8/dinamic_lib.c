#include "dinamic_lib.h"

void main()
{
	int number_menu=0;
	int result;
	int a;
	int b;
	void *dlib;
	int (*add)(int, int);
	int (*div)(int, int);
	int (*mul)(int, int);
	int (*sub)(int, int);
	char *error;
	dlib = dlopen("/home/alexandr/homew/dinamic_lib/libdinamic_lib.so", RTLD_LAZY);
	if((error=dlerror())!=0)
	{
		printf("%s", error);
		exit(1);
	}
	add=dlsym(dlib, "func_add");
	div=dlsym(dlib, "func_div");
	mul=dlsym(dlib, "func_mul");
	sub=dlsym(dlib, "func_sub");
	while(number_menu!=5)
	{
		printf("1) Summation\n2) Subtractions\n3) Multiplications\n4) Divisions\n5) Exit\nInpet number: ");
		scanf("%i", &number_menu);
		if(number_menu==1)
		{
			printf("Input first number: ");
			scanf("%i", &a);
			printf("Input second number: ");
			scanf("%i", &b);
			result=add(a,b);
			printf("result summation: %i\n", result);
		}
		if(number_menu==2)
		{
			printf("Input first number: ");
			scanf("%i", &a);
			printf("Input second number: ");
			scanf("%i", &b);
			result=sub(a,b);
			printf("result subtractions: %i\n", result);
		}
		if(number_menu==3)
		{
			printf("Input first number: ");
			scanf("%i", &a);
			printf("Input second number: ");
			scanf("%i", &b);
			result=mul(a,b);
			printf("result multiplications: %i\n", result);
		}
		if(number_menu==4)
		{
			printf("Input first number: ");
			scanf("%i", &a);
			printf("Input second number: ");
			scanf("%i", &b);
			result=div(a,b);
			printf("result divisions: %i\n", result);
		}
		if(number_menu<1 || number_menu>5) printf("Invaled number! Try again.\n");
	}
	dlclose(dlib);
}