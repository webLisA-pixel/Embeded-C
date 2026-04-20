#include <stdio.h>

int main(void)
{
	int i=0;
	int j=0;
	int n=1;
	int N=0;
	printf("Input size massive:\n");
	scanf("%i", &N);
	int m[N][N];
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			{
				m[i][j]=n;
				n++;
			}
	for(i=0;i<N;i++)
		{
			for(j=0;j<N;j++)
				printf("%i\t", m[i][j]);
			printf("\n");
		}
	return 0;
}