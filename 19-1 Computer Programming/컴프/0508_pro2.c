#include <stdio.h>
#include <string.h>

int main()
{
	int i, cnt[30] = { 0, };
	char str[1000];
	char ch, *p;

	printf("Enter three lines of text:\n");

	for (i = 0; i < 3; i++)
	{
		gets(str);
		for (ch = 'a'; ch <= 'z'; ch++)
		{
			p = str;
			while ((p = strchr(p, ch)) != NULL)
			{
				cnt[ch - 'a']++;
				p++;
			}
		}
		for (ch = 'A'; ch <= 'Z'; ch++)
		{
			p = str;
			while ((p = strchr(p, ch)) != NULL)
			{
				cnt[ch - 'A']++;
				p++;
			}
		}
	}

	printf("\nThe total occurrences of each character:\n");
	for (i = 0; i < 26; i++)
	{	
		printf("%c: %2d\n", 'a' + i, cnt[i]);
	}

	return 0;
}