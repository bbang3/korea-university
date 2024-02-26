#include <stdio.h>
#include <string.h>

typedef struct student
{
	char name[50];
	int stdID;
	int grade;
	char major[50];
}Student;

int main()
{
	FILE *out, *in;
	Student temp, stdList[5];
	int i, tofind;

	out = fopen("student_file.txt", "w");

	for (i = 0; i < 5; i++)
	{
		printf("Name Student_number Grade Major: ");
		scanf("%s %d %d %s", temp.name, &temp.stdID, &temp.grade, temp.major);
		fprintf(out, "%s %d %d %s\n", temp.name, temp.stdID, temp.grade, temp.major);
	}
	fclose(out);


	in = fopen("student_file.txt", "r");
	printf("Insert the student number to find: ");
	scanf("%d", &tofind);

	for (i = 0; i < 5; i++)
	{
		fscanf(in, "%s %d %d %s", stdList[i].name, &stdList[i].stdID, &stdList[i].grade, stdList[i].major);
	}

	for (i = 0; i < 5; i++)
		if (stdList[i].stdID == tofind)
		{
			printf("The major of student with ID %d is %s.\n", tofind, stdList[i].major);
		}
	
	fclose(in);
	return 0;
}