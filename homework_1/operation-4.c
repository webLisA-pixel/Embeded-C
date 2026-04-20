#include <stdio.h>

int main(void)
{
	int n;
	int m;
	printf("Input int number: ");
	scanf("%i", &n);
	printf("Input number not more 256:");
	scanf("%i", &m);
	m=m<<16;
	n=n|m;
	printf("%i\n", n);
	return 0;
}