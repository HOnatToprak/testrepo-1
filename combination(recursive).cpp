#include<stdio.h>
int combination(int a, int b);
int factorial(int a);
int main() 
{
	int a, b;
	printf("iki sayi giriniz:");
	scanf("%d %d",&a, &b);
	printf("\nkombinasyonlari:%d\n", combination(a, b));
	return 0;
}
int factorial(int a)
{
	int sum = 1;
	for (int b = 1; b <= a; b++)
		sum *= b;
	return sum;
}
int combination(int a, int b)
{
	int c;
	c = factorial(a) / (factorial(b)*factorial(a - b));
	return(c);
}
