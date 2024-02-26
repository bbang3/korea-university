#include <stdio.h>

int main()
{
	char inname[50], outname[50], buffer[100];
	FILE *in, *out;
	
	printf("Enter the name of file to read: ");
	scanf("%s", inname);
	printf("Enter the name of file to write: ");
	scanf("%s", outname);

	in = fopen(inname, "r");
	out = fopen(outname, "w");
	 
	while (fgets(buffer, 100, in) != NULL)
	{
		if (buffer[0] != '\n')
			fputs(buffer, out);
	}

	printf("Copy txt complete!\n");
	fclose(in);
	fclose(out);

	return 0;
}