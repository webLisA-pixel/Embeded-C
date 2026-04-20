#include <stdio.h>

int main(void)
{
	int n=1;
	int N=0;
	int i=0;
	printf("Input size massive:\n");
	scanf("%i", &N);
	int m[N];
	for(i=0;i<N;i++)
	{
		m[i]=n;
		n++;
	}
	while(N>0)
	{
		N--;
		printf("%i\t", m[N]);
	}
	printf("\n");
	return 0;
}