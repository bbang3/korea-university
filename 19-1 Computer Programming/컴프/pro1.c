#include <stdio.h>

int main()
{
	int num;
	printf("Input an integer : ");
	scanf("%d", &num);

	if (num % 2 == 0)
	{
		printf("%d is an even integer\n", num);
	}
	if (num % 2 != 0)
	{
		printf("%d is an odd integer\n", num);
	}
	return 0;
}