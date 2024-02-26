#include <stdio.h>

int main()
{
	int a, b, c, d, e;
	int largest, smallest;

	printf("Input 5 Integers : ");
	scanf("%d %d %d %d %d", &a, &b, &c, &d, &e);

	largest = a;
	smallest = a;

	if (largest < b)
		largest = b;
	if (smallest > b)
		smallest = b;

	if (largest < c)
		largest = c;
	if (smallest > c)
		smallest = c;

	if (largest < d)
		largest = d;
	if (smallest > d)
		smallest = d;

	if (largest < e)
		largest = e;
	if (smallest > e)
		smallest = e;

	printf("The largest value is %d\n", largest);
	printf("The smallest value is %d\n", smallest);

	return 0;
}