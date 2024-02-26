#include <stdio.h>


int main()
{
	int a[] = { 1,3,5,7,9,11,13 };
	int *p = a + 1;

	for (int i = 0; i < 7; i++) printf("%d ", a[i]);

	printf("%d ",*p--);
	printf("%d", *p);


	return 0;
}