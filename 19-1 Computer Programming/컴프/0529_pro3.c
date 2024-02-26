#include <stdio.h>
#include <string.h>

typedef struct {
	char word[20];
	int count;
	int isWritten;
}wordutility;

int main()
{
	char str[50];
	char *p;
	int i, j, wcnt = 0, isnew;
	int freq[50] = { 0, };
	wordutility wu[100];

	printf("Enter three lines of text:\n");
	for (i = 0; i < 3; i++)
	{
		gets(str);

		p = strtok(str, " .\n");
		while (p != NULL)
		{
			isnew = 1;
			for (j = 0; j < wcnt; j++)
			{
				if (strcmp(p, wu[j].word) == 0)
				{
					wu[j].count += 1;
					isnew = 0;
					break;
				}
			}
			if (isnew)
			{
				strcpy(wu[wcnt].word, p);
				wu[wcnt].count = 1;
				wu[wcnt].isWritten = 1;
				wcnt++;
			}

			p = strtok(NULL, " .\n");
		}
	}
	printf("\n");
	for (i = 0; i < wcnt; i++)
	{
		printf("\"%s\" appeared %d time%s\n", wu[i].word, wu[i].count, wu[i].count == 1 ? "" : "s");
	}


	return 0;
}