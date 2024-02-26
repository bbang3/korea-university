#include <stdio.h>

void f(int *a, int *b, int *c)
{
	int tmp;
	*a += 1;
	*b *= *c;

	tmp = *b;
	*b = *c;
	*c = tmp;

	return;
}

int main()
{
	int n1, n2, n3;
	printf("Enter three numbers: ");
	scanf("%d %d %d", &n1, &n2, &n3);

	printf("Before: a = %d, b = %d, c = %d\n", n1, n2, n3);
	f(&n1, &n2, &n3);
	printf("After: a = %d, b = %d, c = %d\n", n1, n2, n3);

	return 0;
}