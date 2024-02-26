#include <stdio.h>

int compare(char *str1, char *str2)
{
	while (*str1 != NULL && *str2 != NULL)
	{
		if (*str1 != *str2)
		{
			return 0;
		}
		str1++;
		str2++;
	}
	if (*str1 == NULL && *str2 == NULL) return 1;
	return 0;
}

int main()
{
	char string1[80], string2[80];
	printf("Enter two strings: ");
	scanf("%s %s", string1, string2);
	
	if(compare(string1, string2))
		printf("%s and %s are equal.\n", string1, string2);
	else printf("%s and %s are not equal.\n", string1, string2);

	return 0;
}