#include <stdio.h>

int main(void)
{
	int n;
	printf("print int number: ");
	scanf("%i", &n);
	unsigned char c;
	c=(n&0x000000FF);
	printf("your number = %b\n", c);
	return 0;
}