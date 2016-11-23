#include<stdio.h>
int main() {
	int number;
	int flag = 1;
	while (1) {
		printf("\nsayi giriniz:");
		scanf("%d", &number);
		printf("\nsayinizin asal carpanlari\n------------------------------------------\n");
		for (int bolen1 = 2; bolen1 <= number; bolen1++)
			if (number%bolen1 == 0)
			{
				for (int bolen2 = 2; bolen2 < bolen1; bolen2++)
					if (bolen1%bolen2 == 0)
					{
						flag = 0;
						if(flag==0)
							break;
					}
				if (flag == 1)
					printf("%d - ", bolen1);
				flag = 1;
			}
	}
	return 0;
}