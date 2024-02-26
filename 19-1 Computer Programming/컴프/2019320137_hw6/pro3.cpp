#include <stdio.h>

void concat(char *s1, char *s2)
{
	while (*s1 != NULL) s1++;

	while (*s2 != NULL)
	{
		*s1 = *s2;
		s1++;
		s2++;
	}
	*s1 = NULL;

	return;
}

int main()
{
	char string1[80], string2[80];
	printf("Enter two strings: ");
	scanf("%s %s", string1, string2);

	concat(string1, string2);
	printf("%s\n", string1);

	return 0;
}