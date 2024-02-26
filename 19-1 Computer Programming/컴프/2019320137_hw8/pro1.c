#include <stdio.h>

int main()
{
	unsigned int n;
	int cnt = 0;

	printf("Enter an integer: ");
	scanf("%u", &n);
	
	while (n > 0)
	{
		if(n & 1)
			cnt++;
		n = n >> 1;
	}
	printf("The total number of 1s in the bit representation is %s\n", cnt % 2 == 1 ? "odd" : "even");

	return 0;
}