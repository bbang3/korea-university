#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void add(int m1[5][6], int m2[5][6], int result[5][6])
{
	int i, j;

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 6; j++)
		{
			result[i][j] = m1[i][j] + m2[i][j];
		}
	}

	return;
}

int main()
{
	srand((unsigned int)time(NULL));
	int i, j;
	int A[5][6], B[5][6], result[5][6];

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 6; j++)
		{
			A[i][j] = rand() % 100 + 1;
			B[i][j] = rand() % 100 + 1;
		}
	}

	printf("matrix1:\n");
	for (i = 0; i < 5; i++)
	{ 
		for (j = 0; j < 6; j++)
			printf("%3d\t", A[i][j]);
		printf("\n");
	}

	printf("\nmatrix2:\n");
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 6; j++)
			printf("%3d\t", B[i][j]);
		printf("\n");
	}

	add(A, B, result);

	printf("\nresult:\n");
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 6; j++)
			printf("%3d\t", result[i][j]);
		printf("\n");
	}

	return 0;
}