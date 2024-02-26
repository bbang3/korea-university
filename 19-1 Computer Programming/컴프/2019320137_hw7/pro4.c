#include <stdio.h>
#include <string.h>

void bubbleSort(char a[][50])
{
	int i, j;

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9 - i; j++)
		{
			if (strcmp(a[j], a[j + 1]) > 0)
			{
				char tmp[50];
				strcpy(tmp, a[j]);
				strcpy(a[j], a[j + 1]);
				strcpy(a[j + 1], tmp);
			}
		}
	}
	return;
}

int main()
{
	char strarr[10][50];
	int i;

	for (i = 0; i < 10; i++)
	{
		printf("Enter a string: ");
		scanf("%s", &strarr[i]);
	}
	bubbleSort(strarr);

	printf("\nThe strings in sorted order are:\n");
	for (i = 0; i < 10; i++)
	{
		printf("%s\n", strarr[i]);
	}

	return 0;
}