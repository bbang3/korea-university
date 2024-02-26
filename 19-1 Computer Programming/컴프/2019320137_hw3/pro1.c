#include <stdio.h>

int main()
{
	int i = 0, largest = 0;
	int num;
	do
	{
		printf("Enter the number : ");
		scanf("%d", &num);
		if (i == 0)
		{
			largest = num;
		}
		else if (num > largest)
			largest = num;
		i++;
	} while (i < 5);

	printf("Largest is %d\n", largest);

	return 0;
}