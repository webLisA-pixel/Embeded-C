#include <stdio.h>

int main(void)
{
	int n;
	int m=0;
	int k=8;
	char c;
	printf("Input int number not more 256: ");
	scanf("%i", &n);
	while(k>0)
	{
		if((n&0x00000001)!=0)
			m++;
		n=n>>1;
		k--;
	}
	printf("%i\n", m);
	return 0;
}