#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main()
{
	srand((unsigned int)time(NULL));
	int i, arr[10];
	for (i = 0; i < 10; i++)
	{
		arr[i] = rand() % 100 + 1;
		printf("%d ", *(arr + i));
	}
	printf("\n");
	for (i = 9; i >= 0; i--)
	{
		printf("%d ", *(arr + i));
	}
	
	return 0;
}