#include<stdio.h>
int basamakbul(int a);
int main() 
{
	int a;
	printf("sayi giriniz:");
	scanf("%d", &a);
	printf("\nsayiniz %d basamakli\n", basamakbul(a));
	return 0;
}
int basamakbul(int a)
{
	int toplam;
	for (toplam = 01; a > 10; toplam++)
		a /= 10;
	return toplam;
}

