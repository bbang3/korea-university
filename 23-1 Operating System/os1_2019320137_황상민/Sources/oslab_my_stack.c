#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/linkage.h>

#define MAX_SIZE 100 // maximum size of the stack

int top = -1; // variable that points the top of stack
int stack[MAX_SIZE];

// print all stack elements
void print_stack(void){
	int i;
	printk("Stack Top ---------------------\n");
	for(i=top; i >= 0; i--)
		printk("%d\n", stack[i]);
	printk("Stack Bottom ------------------\n");
}

SYSCALL_DEFINE1(os2023_push, int, a){
	int i, is_duplicate = 0;
	for(i=top; i >= 0; i--) { // check if there is a duplicate element
		if(stack[i] == a) {
			is_duplicate = 1;
			break;
		}
	}

	if(!is_duplicate)
		stack[++top] = a; // push element at the top of stack
	
	printk("\033[0;33m[System Call] os2023_push \033[0m:\n");
	print_stack();
}

SYSCALL_DEFINE0(os2023_pop){
	int ret = stack[top--]; // pop element at the top of stack
	
	printk("\033[0;33m[System Call] os2023_pop \033[0m:\n");
	print_stack();

	return ret; // return popped element
}

