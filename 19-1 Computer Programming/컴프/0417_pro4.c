#include <stdio.h>

void stringReverse(char strArray[])
{
	if (strArray[0] == NULL) return; 
	
	stringReverse(strArray + 1);
	printf("%c", strArray[0]);

	return;
}

int main()
{
	char strArray[30] = "Print this string backward.";

	printf("%s\n", strArray);
	stringReverse(strArray);
	printf("\n");

	return 0;
}