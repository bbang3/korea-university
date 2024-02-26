#include <stdio.h>

int main()
{
	int i, j, k;

	for (i = 1; i <= 9; i++)
	{
		if (i <= 5)
		{
			for (j = 1; j <= 5 - i; j++)
				printf(" ");
			for (k = 1; k <= 2 * i - 1; k++)
				printf("*");
		}
		else
		{
			for (j = 1; j <= i - 5; j++)
				printf(" ");
			for (k = 1; k <= 2 * (10 - i) - 1; k++)
				printf("*");
		}
		printf("\n");
	}

	return 0;
}