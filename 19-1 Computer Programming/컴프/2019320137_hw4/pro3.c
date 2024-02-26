#include <stdio.h>

int ulcase(char c)
{
	return c >= 'A' && c <= 'Z';
}

int main()
{
	char c;
	printf("Enter the character : ");
	scanf("%c", &c);
	if (ulcase(c))
		printf("%c : Uppercase\n", c);
	else
		printf("%c : Lowercase\n", c);

	return 0;
}