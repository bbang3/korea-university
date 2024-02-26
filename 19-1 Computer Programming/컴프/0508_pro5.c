#include <stdio.h>
#include <string.h>

int main()
{
	char words[50][50], str[50];
	char *p;
	int i, j, wcnt = 0, isnew = 0;
	int freq[50] = { 0, };
	
	printf("Enter three lines of text:\n");
	for (i = 0; i < 3; i++)
	{
		gets_s(str, sizeof(str));

		p = strtok(str, " .\n");
		while (p != NULL)
		{
			isnew = 1;
			for (j = 0; j < wcnt; j++)
			{
				if (strcmp(p, words[j]) == 0)
				{
					freq[j]++;
					isnew = 0;
					break;
				}
			}
			if (isnew)
			{
				strcpy(words[wcnt], p);
				freq[wcnt]++;
				wcnt++;
			}

			p = strtok(NULL, " .\n");
		}
	}
	printf("\n");
	for (i = 0; i < wcnt; i++)
	{
		printf("\"%s\" appeared %d time%s\n", words[i], freq[i], freq[i] == 1 ? "" : "s");
	}


	return 0;
} 