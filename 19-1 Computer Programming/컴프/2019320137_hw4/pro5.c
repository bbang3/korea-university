#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int flip()
{
	return rand() % 2;
}

int main()
{
	srand((unsigned int)time(NULL));
	int i;
	int hcnt = 0, tcnt = 0;
	for (i = 1; i <= 100; i++)
	{
		if (flip())
		{
			printf("Heads ");
			hcnt += 1;
		}
		else
		{
			printf("Tails ");
			tcnt += 1;
		}
		if (i % 10 == 0) printf("\n");
	}
	printf("\n");
	printf("The total number of Heads was %d\n", hcnt);
	printf("The total number of Tails was %d\n", tcnt);
	return 0;
}