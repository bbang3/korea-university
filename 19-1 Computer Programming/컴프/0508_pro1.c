#include <stdio.h>
#include <stdlib.h>

int main()
{
	int i, sum = 0;
	char str[30];

	for (i = 0; i < 4; i++)
	{
		printf("Enter an integer string: ");
		scanf("%s", str);

		sum += atoi(str);
	}
	printf("\nThe total of the values is %d\n", sum);

	return 0;
}