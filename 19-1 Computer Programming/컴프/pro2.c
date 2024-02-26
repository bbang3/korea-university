#include <stdio.h>

int main()
{
	int x, y;

	printf("Enter two numbers : ");
	scanf("%d %d", &x, &y);

	printf("The sum is %d\n", x + y);
	printf("The product is %d\n", x * y);
	printf("The difference is %d\n", x - y);
	printf("The quotient is %d\n", x / y);
	printf("The remainder is %d\n", x % y);

	return 0;
}