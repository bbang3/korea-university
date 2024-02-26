#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct stduent
{
	char name[80];
	int stdnum;
	float avg;
	int kor;
	int math;
	int eng;
}Student;

void bubbleSort(Student stdList[])
{
	int i, j;
	Student temp;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4 - i; j++)
		{
			if (stdList[j].avg < stdList[j + 1].avg)
			{
				temp = stdList[j];
				stdList[j] = stdList[j + 1];
				stdList[j + 1] = temp;
			}
		}
	}
	return;
}

int main()
{
	FILE *in, *out;
	Student stdList[5] = { 0, };
	char buffer[300] = { 0, }, str[5][100] = { 0, };
	char *p;
	int i, j;

	in = fopen("stdlist.txt", "r");
	fread(buffer, sizeof(char), 300, in);
	fclose(in);

	i = 0;
	p = strtok(buffer, "\n");
	while (p != NULL)
	{
		strcpy(str[i++], p);
		p = strtok(NULL, "\n");
	}

	for (i = 0; i < 5; i++)
	{
		p = strtok(str[i], "\t ");
		strcpy(stdList[i].name, p);
		p = strtok(NULL, "\t ");
		stdList[i].stdnum = atoi(p);
		p = strtok(NULL, "\t ");
		stdList[i].avg = atof(p);
		p = strtok(NULL, "\t ");
		stdList[i].kor = atoi(p);
		p = strtok(NULL, "\t ");
		stdList[i].math = atoi(p);
		p = strtok(NULL, "\t ");
		stdList[i].eng = atoi(p);
	}

	out = fopen("resultlist.txt", "w");
	bubbleSort(stdList);
	printf("Name     \t\t ID\t Avg\t\t KOR\t MATH\t ENG\n");
	for (i = 0; i < 5; i++)
	{
		printf("%s\t\t %d\t %f\t %d\t %d\t %d\n", stdList[i].name, stdList[i].stdnum, stdList[i].avg, stdList[i].kor, stdList[i].math, stdList[i].eng);
		fprintf(out, "%s\t\t %d\t %f\t %d\t %d\t %d\n", stdList[i].name, stdList[i].stdnum, stdList[i].avg, stdList[i].kor, stdList[i].math, stdList[i].eng);
	}
	fclose(out);

	return 0;
}