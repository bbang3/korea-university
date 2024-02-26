#include <stdio.h>

int main()
{
	int num, loop;
	int digit;
	
	printf("Decimal\t\tBinary\n");

	for (loop = 0; loop <= 15; loop++)
	{
		printf("%d\t\t", loop);
		num = loop;
		for (digit = 8; digit > 0; digit /= 2)
		{
			printf("%d", num / digit > 0 ? 1 : 0);
			num = num % digit;
		}
		printf("\n");
	}

	return 0;
}