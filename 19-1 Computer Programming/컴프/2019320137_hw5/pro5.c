#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void getminmax(int matrix[5][5])
{
	int i, j, di, dj;
	int sum = 0, maxsum = -1, minsum = -1;
	int maxi = -1, maxj = -1, mini = -1, minj = -1;


	for (i = 0; i <= 3; i++)
		for (j = 0; j <= 3; j++)
		{
			sum = 0;
			for (di = 0; di <= 1; di++)
				for (dj = 0; dj <= 1; dj++)
				{
					sum += matrix[i + di][j + dj];
				}

			if (i == 0 && j == 0)
			{
				maxsum = sum;
				maxi = i;
				maxj = j;

				minsum = sum;
				mini = i;
				minj = j;
			}
			else if (sum > maxsum)
			{
				maxsum = sum;
				maxi = i;
				maxj = j;
			}
			else if (sum < minsum)
			{
				minsum = sum;
				mini = i;
				minj = j;
			}
		}


	printf("The largest square value : %d (%d,%d)\n", maxsum, maxi, maxj);
	printf("The smallest square value : %d (%d,%d)\n", minsum, mini, minj);

	return;
}

int main()
{
	srand((unsigned int)time(NULL));
	int matrix[5][5];
	int i, j;

	printf("Matrix\n");
	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			matrix[i][j] = rand() % 9 + 1;
			printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}

	getminmax(matrix);

	return 0;
}