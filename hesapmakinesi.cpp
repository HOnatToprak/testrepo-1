#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
int main()
{
	int a;
	int b;
	int c;
	while(1)
	{
		do
		{
			printf("\n\n\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/");
			printf("\n\n1 for sum\n2 for substraction\n3 for multiplication\n4 for division");
			printf("\n=============\n");
			printf("Pick a calculation method:   ");
			scanf("%d" , &c);
			if(c<1 || c>4)
			{
				printf("Please enter in range of 1-4");
				printf("\n\nPress any button to clear...");
				getch();
				system("cls");
			}
		}
		while(c<1 || c>4);
			
	
		printf("\n\nSelected method is ");
		switch(c)
	
		{
	
			case 1:printf("sum");break;
			case 2:printf("substraction");break;
			case 3:printf("multiplication");break;
			case 4:printf("division");break;
		}
    
    	printf("\nNow enter two integer:     ");
    	scanf("%d%d", &a , &b);
    	printf("\nThe answer of ");
    	switch(c)
	
		{
	
			case 1:printf("sum");break;
			case 2:printf("substraction");break;
			case 3:printf("multiplication");break;
			case 4:printf("division");break;
    	}
    	printf(" is     ");
    	switch(c)
	
		{
	
			case 1:printf("%d\n\n", a+b);break;
			case 2:printf("%d\n\n" , a-b);break;
			case 3:printf("%d\n\n" , a*b);break;
			case 4:printf("%.2f\n\n" , (float)a/b);break;
    	}
   
    	printf("Press any button to clear...");
		getch();
		system("cls")
    }
return 0;	
}
