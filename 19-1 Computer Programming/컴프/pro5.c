#include <stdio.h>

int main()
{
	int a, b;
	int quo, rem;
	printf("Input two integers : ");
	scanf("%d %d", &a, &b);
	quo = a / b;
	rem = a % b;
	printf("%d = %d * %d + %d\n", a, b, quo, rem);

	if (quo % 2 == 0)
		printf("quotient is even\n");
	if (quo % 2 != 0)
		printf("quotient is odd\n");
	if (rem % 2 == 0)
		printf("remainder is even\n");
	if (rem % 2 != 0)
		printf("remainder is odd\n");
	return 0;
}