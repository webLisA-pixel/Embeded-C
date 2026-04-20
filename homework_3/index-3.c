#include <stdio.h>

int main(void)
{
	int n[10];
	int m=0;
	int *p;
	for(int i=0;i<10;i++)
		n[i]=i+1;
	p = (int *)(&n);
	while(m<10)
	{
		printf("%i\n", *p);
		p++;
		m++;
	}
	return 0;
}