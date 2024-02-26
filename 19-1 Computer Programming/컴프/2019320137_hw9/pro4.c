#include <stdio.h>
#include <string.h>

typedef struct {
	int lowercount[26];
	int uppercount[26];
}chcount;

int main()
{
	int i, j;
	char str[1000];
	char ch, ch2, *p;
	chcount chcnt[3];

	printf("Enter three lines of text:\n");

	for (i = 0; i < 3; i++)
	{
		gets(str);

		for (ch = 'a'; ch <= 'z'; ch++)
		{
			chcnt[i].lowercount[ch - 'a'] = 0;
			p = str;
			while ((p = strchr(p, ch)) != NULL)
			{
				chcnt[i].lowercount[ch - 'a']++;
				p++;
			}
		}
		for (ch = 'A'; ch <= 'Z'; ch++)
		{
			chcnt[i].uppercount[ch - 'A'] = 0;
			p = str;
			while ((p = strchr(p, ch)) != NULL)
			{
				chcnt[i].uppercount[ch - 'A']++;
				p++;
			}
		}
	}

	printf("\nPrint Line\n\n");
	printf("0\t\t1\t\t2\t\t\n");

	ch = 'a'; ch2 = 'A';
	for (i = 0; i < 26; i++)
	{
		for (j = 0; j < 3; j++)
		{
			printf("%c : %d  %c : %d\t", ch + i, chcnt[j].lowercount[i], ch2 + i, chcnt[j].uppercount[i]);
		}
		printf("\n");
	}
	return 0;
}