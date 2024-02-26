#include <stdio.h>

int main()
{
	int base = -1, exp = -1, i;
	int result = 1;
	while (base < 0)
	{
		printf("Enter the base number : ");
		scanf("%d", &base);
	}
	while (exp < 0)
	{
		printf("Enter the exponent number : ");
		scanf("%d", &exp);
	}
	for (i = 0; i < exp; i++)
	{
		result *= base;
	}
	printf("%d^%d is %d\n", base, exp, result);
	return 0;
}