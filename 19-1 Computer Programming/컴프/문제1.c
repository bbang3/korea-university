#include <stdio.h>

static void func(int n) {
	int x = n + 1;
	printf("x = %d\n", x);
	if (x % 2) {
		static int n = 10;
		printf("n = %d\n", n);
		n++;
	}
	else {
		static int n = 20;
		printf("n = %d\n", n);
		n++;
	}
}

int main() {
	func(10);
	func(11);
	func(12);
	func(13);
}

//x = 11
//n = 10
//x = 12
//n = 20
//x = 13
//n = 11
//x = 14
//n = 21