#include <stdio.h>

int main()
{
	int n = -1;
	int sum = 0, largest = 0, smallest = 0, oddcnt = 0, evencnt = 0, negcnt = 0;

	printf("Input the number : ");
	scanf("%d", &n);
	if (n != 0)
	{

		largest = n;
		smallest = n;
		sum = n;

		if (n > 0)
		{
			if (n % 2 == 0) evencnt++;
			else oddcnt++;
		}
		else negcnt++;
	}
	while (n != 0)
	{
		printf("Input the number : ");
		scanf("%d", &n);

		if (n != 0)
		{
			sum += n;

			if (n > largest) largest = n;
			if (n < smallest) smallest = n;

			if (n > 0)
			{
				if (n % 2 == 0) evencnt++;
				else oddcnt++;
			}
			else negcnt++;
		}
	}
	printf("------------------------\n");
	printf("Sum : %d\n", sum);
	printf("Largest : %d\n", largest);
	printf("Smallest : %d\n", smallest);
	printf("Odd number count : %d\n", oddcnt);
	printf("Even number count : %d\n", evencnt);
	printf("Negative number count : %d\n", negcnt);

	return 0;
}