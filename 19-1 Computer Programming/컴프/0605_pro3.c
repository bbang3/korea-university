#include <stdio.h>

int main()
{
	FILE *in1, *in2, *out;
	int i, j;
	int mat1[2][3], mat2[2][3], result[2][3];

	in1 = fopen("input1.txt", "r");
	in2 = fopen("input2.txt", "r");

	printf("input1 matrix:\n");
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fscanf(in1, "%d", &mat1[i][j]);
			printf("%d\t", mat1[i][j]);
		}
		printf("\n");
	}

	printf("\ninput2 matrix:\n");
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 3; j++)
		{
			fscanf(in2, "%d", &mat2[i][j]);
			printf("%d\t", mat2[i][j]);
		}
		printf("\n");
	}

	out = fopen("result.txt", "w");
	printf("\nresult matrix:\n");
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 3; j++)
		{
			result[i][j] = mat1[i][j] + mat2[i][j];

			printf("%d\t", result[i][j]);
			fprintf(out, "%d\t", result[i][j]);
		}
		printf("\n");
		fprintf(out, "\n");
	}


	fclose(in1);
	fclose(in2);
	fclose(out);
	return 0;
}