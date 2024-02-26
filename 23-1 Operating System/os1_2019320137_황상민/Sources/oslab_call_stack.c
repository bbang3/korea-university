#include <unistd.h>
#include <stdio.h>

// system call numbers
#define MY_STACK_PUSH 335
#define MY_STACK_POP 336

int main() {
	printf("Push 1\n");
	syscall(MY_STACK_PUSH, 1); // push 1

	for(int i=1;i<=3;i++) { // push 1, 2, 3
		printf("Push %d\n", i);
		syscall(MY_STACK_PUSH, i); // pass parameter (i) to PUSH system call
	}
	for(int i=3;i>=1;i--) { // pop 3, 2, 1
		printf("Pop %d\n", i);
		int ret = syscall(MY_STACK_POP);
	}
	return 0;
}
