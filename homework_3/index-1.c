#include <stdio.h>

int main(void)
{
	int n;
	int m;
	char *p;
	printf("input int number: ");
	scanf("%i", &n);
	printf("input number for change not more 256 = ");
	scanf("%i", &m);
	p = (char *)(&n);
	printf("your number = %i\n", n);
	printf("changing number = %i\n", m);
	p=p+2;
	printf("\n");
	*p=*p+m;
	printf("%i\n", n);
	return 0;
}