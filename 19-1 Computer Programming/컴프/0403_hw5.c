#include <stdio.h>

int main()
{
	int inum = 0, quan = 0;
	double total = 0;
	printf("Enter paris of item numbers and quantities\n");
	printf("Enter -1 for the item number to end input\n");

	while (1)
	{
		scanf("%d", &inum);
		if (inum == -1)
			break;
		scanf("%d", &quan);

		switch (inum)
		{
		case 1:
			total += 2.98 * quan;
			break;
		case 2:
			total += 4.5 * quan;
			break;
		case 3:
			total += 9.98 * quan;
			break;
		case 4:
			total += 4.49 * quan;
			break;
		case 5:
			total += 6.87 * quan;
			break;
		default:
			printf("Invalid product code : %d\n", inum);
			printf("Quantity : %d\n", quan);
		}
	}

	printf("The total retail value was : %.2lf\n", total);

	return 0;
}