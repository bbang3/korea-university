#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 5

typedef struct {
	char name[80];
	int stdnum;
} student;

void bubbleSort(student arr[])
{
	int i, j;
	student temp;

	for (i = 0; i < MAX - 1; i++)
	{
		for (j = 0; j < MAX - i - 1; j++)
		{
			if (arr[j].stdnum > arr[j + 1].stdnum)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}
}

int main()
{
	int i;
	student stdInfo[5];
	char str[5][50];

	for (i = 0; i < MAX; i++)
	{
		printf("<%d> Enter student name / student number: ", i + 1);
		gets(str[i]);
	}

	for (i = 0; i < MAX; i++)
	{
		strcpy(stdInfo[i].name, strtok(str[i], "/"));
		stdInfo[i].stdnum = atoi(strtok(NULL, "/"));
	}
	bubbleSort(stdInfo);

	printf("\nID\t\t Name\n");
	for (i = 0; i < MAX; i++)
	{
		printf("%d\t %s\n", stdInfo[i].stdnum, stdInfo[i].name);
	}
	return 0;
}