#include<stdio.h>

//define extra functions here if needed

int dig(int p) {
	int cnt = 0;
	while (p) {
		p /= 10;
		cnt++;
	}
	return cnt;
}

int is_prime(int k) {
	int i;
	for (i = 2; i*i <= k; i++) {
		if (k%i == 0) return 0;
	}
	return 1;
}

int is_circular_prime(int n) {
	int i;
	int digit = dig(n);
	int newnum = n, t;
	for (i = 0; i<digit; i++) {
		if (!is_prime(newnum)) break;
		t = newnum % 10;
		newnum /= 10;
		newnum += t * (10 << (digit - 1));
	}
	if (i == digit) return 1;
}

int main() {
	int num;
	printf("Enter number: ");
	scanf("%d", &num);
	if (is_circular_prime(num)) printf("yes");
	else printf("no");
}