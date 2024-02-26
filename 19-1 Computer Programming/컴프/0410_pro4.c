#include <stdio.h>

int reverse(int num)
{
	int result = 0;

	while (num > 0)
	{
		result = result * 10 + num % 10;
		num /= 10;
	}
	return result;
}

int main()
{
	int num;
	printf("Enter a number between 1 and 9999 : ");
	scanf("%d", &num);

	printf("The number with its digits reversed is : %d\n", reverse(num));
	return 0;
}