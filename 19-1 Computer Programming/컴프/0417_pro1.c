#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
	srand((unsigned int)time(NULL));

	int arr[20], check[21] = { 0, }; // n�̶�� ���ڰ� �������� ��� check[n] = 1(true)�� ���� �����մϴ�.
	int idx = 0, num;
	
	num = rand() % 20 + 1;
	while (check[num] == 0)
	{
		arr[idx++] = num;
		check[num] = 1;
		num = rand() % 20 + 1;
	}

	printf("Nonrepetitive array values are:\n");
	for (int i = 0; i < idx; i++)
	{
		printf("Array[ %d ] = %d\n", i, arr[i]);
	}

	return 0;
}