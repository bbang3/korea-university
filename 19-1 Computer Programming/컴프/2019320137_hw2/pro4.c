#include <stdio.h>

int main()
{
	int a, b;
	printf("Input two integers : ");
	scanf("%d %d", &a, &b);

	if (a % b == 0)
	{
		printf("%d is a multiple of %d by a factor of %d\n", a, b, a / b);
	}
	else
	{
		printf("%d is not a multiple of %d\n", a, b);
	}

	return 0;
}