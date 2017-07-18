#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int main() {
	int size;
	int cnt = 1;
	int x=0;
	int y=0;
	int a = 0,b = 0, c=0,d=0;
	int reverse = 0;
	scanf("%d", &size);
	printf("\n\n");
	int truesize = size;
	int **arr = (int**)calloc(size , sizeof(int*));
	for (a = 0; a < size; a++)
		*(arr + a) = (int*)calloc(size , sizeof(int));


	while (cnt <= pow(truesize, 2)) {
		arr[y][x] = cnt;
		cnt++;


		if (!reverse && x < size-1) {
			x++;
		}
		else if (!reverse) {
			y++;
			if (y == size-1)
				reverse = 1;
		}
		else if (x > truesize - size)
			x--;
		else if (reverse) {
			if (y == truesize - (size - 1)) {
			reverse = 0;
			size--;
			x++;
			}
			else
				y--;
			
		}

	}



	for (b = 0; b < truesize; b++) {
		for (c = 0; c < truesize; c++)
			printf("%-5d", arr[b][c]);
		printf("\n");
	}

	return 0;
}
