#include <stdio.h>

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
	unsigned int number, pow, result;

	printf("Enter two integers: ");
	scanf("%u %u", &number, &pow);

	printf("number:\n");
	printf("%7d = ", number);
	printbin(number); printf("\n");

	printf("pow:\n");
	printf("%7d = ", pow);
	printbin(pow); printf("\n");

	result = number << pow;
	printf("%d * 2^%d = %d\n", number, pow, result);
	printf("\%7d = ", result);
	printbin(result); printf("\n");

	return 0;
}