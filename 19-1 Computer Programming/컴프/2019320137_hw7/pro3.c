#include <stdio.h>
#include <string.h>

int main()
{
	int i, lencnt[31] = { 0, };
	char str[1000];
	char ch, *p;

	printf("Enter three lines of text:\n");

	for (i = 0; i < 3; i++)
	{
		gets_s(str);

		p = strtok(str, " .\n");
		while (p != NULL)
		{
			lencnt[strlen(p)]++;
			p = strtok(NULL, " .\n");
		}
	}

	printf("\n");
	for (i = 1; i <= 30; i++)
	{
		if (lencnt[i] == 0) continue;

		printf("%d word%s of length %d\n", lencnt[i], lencnt[i] == 1 ? "" : "s", i);
	}

	return 0;
}