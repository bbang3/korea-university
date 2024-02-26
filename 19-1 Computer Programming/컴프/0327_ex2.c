#include <stdio.h>

int main()
{
	int n = -1;
	int cnt = 1, fac = 1;
	while (n < 0)
	{
		printf("Enter a positive integer : ");
		scanf("%d", &n);
	}
	while (cnt <= n)
	{
		fac *= cnt;
		cnt++;
	}

	printf("%d! is %d\n", n, fac);

	return 0;
}