#include "static_lib.h"
#include <stdio.h>

void main()
{
	int number_menu=0;
	int result;
	int a;
	int b;
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
				result=func_add(a,b);
				printf("result summation: %i\n", result);
			}
			if(number_menu==2)
			{
				printf("Input first number: ");
				scanf("%i", &a);
				printf("Input second number: ");
				scanf("%i", &b);
				result=func_sub(a,b);
				printf("result subtractions: %i\n", result);
			}
			if(number_menu==3)
			{
				printf("Input first number: ");
				scanf("%i", &a);
				printf("Input second number: ");
				scanf("%i", &b);
				result=func_mul(a,b);
				printf("result multiplications: %i\n", result);
			}
			if(number_menu==4)
			{
				printf("Input first number: ");
				scanf("%i", &a);
				printf("Input second number: ");
				scanf("%i", &b);
				result=func_div(a,b);
				printf("result divisions: %i\n", result);
			}
			if(number_menu<1 || number_menu>5) printf("Invaled number! Try again.\n");
		}
}