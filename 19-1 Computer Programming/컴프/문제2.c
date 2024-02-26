#include <stdio.h>
void mystery(int n) {
	if (n != 0) {
		mystery(n / 10);
		printf("%d", !(n % 2));
	}
}
int main(void) {
	mystery(23594);
	printf("\n");
	return 0;
}// 10001