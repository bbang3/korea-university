#include <stdio.h>

unsigned int reverseBits(unsigned int n)
{
	unsigned int ret = 0;
	int i;
	
	for (i = 0; i <= 15; i++)
	{
		ret = ret << 1;
		ret += (n >> i) & 1;
	}
	return ret;
}

void printbin(unsigned int num)
{
	int i;
	for (i = 15; i >= 0; i--)
	{
		if (i == 7) printf(" ");
		printf("%d", (num >> i) & 1);
	}
	printf("\n");
}

int main()
{
	unsigned int num, rev;

	printf("Enter an unsigned integer: ");
	scanf("%u", &num);
	printf("\n");

	printf("Before bits are reversed:\n");
	printf("%7u = ", num);
	printbin(num); printf("\n");


	rev = reverseBits(num);
	printf("After bits are reversed:\n");
	printf("%7u = ", rev);
	printbin(rev); printf("\n");

	return 0;
}