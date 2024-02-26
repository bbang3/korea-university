#include <stdio.h>

int perfect(int num)
{
	int i, sum = 0;
	for (i = 1; i < num; i++)
	{
		if (num % i == 0)
		{
			sum += i;
		}
	}
	
	return sum == num;
}

int main()
{
	int n;
	printf("For the integers from 1 to 1000:\n");
	for (n = 2; n <= 1000; n++)
	{
		if (perfect(n))
		{
			printf("%d is perfect\n", n);
		}
	}

	return 0;
}