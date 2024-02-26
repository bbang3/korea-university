#include <stdio.h>

int add(int n1, int n2)
{
	return n1 + n2;
}

int sub(int n1, int n2)
{
	return n1 - n2;
}

int mul(int n1, int n2)
{
	return n1 * n2;
}
int main()
{
	int(*func[3])(int, int) = { add, sub, mul };
	char op[3] = { '+','-','*' };
	int sel, n1, n2, result;

	printf("0.Addition\n");
	printf("1.Subtraction\n");
	printf("2.Multiplication\n");
	printf("3.End\n");

	printf("select the operation: ");
	scanf("%d", &sel);

	if (sel == 3)
	{
		printf("Exit\n");
		return 0;
	}

	printf("Enter the two numbers: ");
	scanf("%d %d", &n1, &n2);

	result = func[sel](n1, n2);
	printf("\n\t%d %c %d = %d\n", n1, op[sel], n2, result);

	return 0;
}