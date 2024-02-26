#include <stdio.h>

int main()
{
	int n, sevencnt = 0, tmp;
	printf("Enter a 5-digit number : ");
	scanf("%d", &n);
	tmp = n;

	if (n > 99999) return -1;
	if (n < 0) return -1;

	while (tmp > 0)
	{
		if (tmp % 10 == 7) sevencnt++;
		tmp /= 10;
	}
	printf("The number %d has %d seven(s) in it\n", n, sevencnt);

	return 0;
}