#include <stdio.h>
#include <math.h>

double hypotenuse(double n1, double n2)
{
	return sqrt(n1 * n1 + n2 * n2);
}

int main()
{
	double a, b;
	int i;
	for (i = 0; i < 3; i++)
	{
		printf("Enter the sides of the triangle : ");
		scanf("%lf %lf", &a, &b);
		printf("Hypotenuse : %.1lf\n", hypotenuse(a, b));
	}
	return 0;
}