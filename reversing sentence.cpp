#include<stdio.h>
int main()
{
	char x[200];
	char y[200];
	int i;
	int j;
	fgets(x , sizeof(x) , stdin);
	for (i = 199; x[i] != '\n'; i--);//cümledeki karakter sayisi bulma
	printf("\n");
	for (j = i; j >= 0; j--) 
	{
			y[i - j] = x[j];
			printf("%c", y[i - j]);
	}
	
	printf("\ni  :  %d", i);
	printf("\nj  :  %d\n", j);
	return 0;
}
