#include <stdio.h>

int main()
{
	int a, b;
	int s, e, sum = 0;
	printf("Enter two integers : ");
	scanf("%d %d", &a, &b);
	
	if (a <= b)
	{
		s = a;
		e = b;
	}
	else 
	{
		s = b;
		e = a;
	}

	while (s <= e)
	{
		sum += s;
		s++;
	}
	printf("The sum of all integers between %d and %d is %d\n", a, b, sum);
	return 0;
}