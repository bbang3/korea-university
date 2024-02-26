#include <stdio.h>

int is_prime(int n)
{
	int i;
	for (i = 2; i < n; i++)
		if (n % i == 0) return 0;

	return 1;
}

int rotate(int n)
{
	int digit = 1, num = n;
	while (num > 0) 
	{ 
		digit *= 10; 
		num /= 10; 
	}
	return (n / 10) + (n % 10) * (digit / 10);
}

int is_circular_prime(int n) {
	int flag = is_prime(n);
	int r;

	if (!flag) return 0;
	r = rotate(n);
	while (r != n)
	{
		if (!is_prime(r)) return 0;
		r = rotate(r);
	}
	return 1;
}
int main() {
	int num;
	printf("Enter number: ");
	scanf("%d", &num);
	if (is_circular_prime(num))
		printf("%d is a circular prime\n", num);
	else
		printf("%d is not circular prime\n", num);
}