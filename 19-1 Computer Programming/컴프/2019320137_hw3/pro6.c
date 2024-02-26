#include <stdio.h>

int main()
{
	double pi = 0.0;
	double num = 4.0;
	double denom = 1.0;
	long int loop;
	long int accuracy;

	accuracy = 900000;

	printf("Accuracy set at : %d\n", accuracy);
	printf("Term\t\t pi\n");

	for (loop = 1; loop <= accuracy; loop++)
	{
		if (loop % 2 == 1) pi += (num / denom);
		else pi -= (num / denom);

		if (loop % 50000 == 0)
			printf("%d\t\t %lf\n", loop, pi);

		denom += 2;
	}

	return 0;
}