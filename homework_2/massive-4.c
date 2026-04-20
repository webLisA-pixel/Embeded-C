#include <stdio.h>

int main(void)
{
    int n;
    printf("Input size massive n = \n");
    scanf("%i", &n);
    int a[n][n];
    int k = 1;
    int i = 0;
    int j = 0;
    while (k <= n*n)
    {
        a[i][j] = k;
        if (i <= j + 1 && i + j < n - 1)
            ++j;
        else if (i < j && i + j >= n - 1)
            ++i;
        else if (i >= j && i + j > n - 1)
            --j;
        else 
            --i;
        ++k;
    }
    for (int i = 0; i < n; ++i){
        for (int j = 0; j < n; ++j)
        	printf("%i\t", a[i][j]);
        printf("\n");
    }  
}