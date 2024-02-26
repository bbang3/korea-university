#include <stdio.h>
#include <stdlib.h>
struct Node {
	int id;
	int count;
	struct Node* next;
};
typedef struct Node* NodePtr;

NodePtr head = NULL;

void print_report() {
	for (NodePtr p = head; p != NULL; p = p->next) {
		printf("id: %d (%d)\n", p->id, p->count);
	}
}

void record_entry(int id)
{
	for(NodePtr p = head; p != NULL; p = p->next)
		if (p->id == id) {
			p->count++;
			return;
		}

	NodePtr newNode = (NodePtr)malloc(sizeof(struct Node));
	newNode->id = id;
	newNode->count = 1;
	newNode->next = NULL;
	if (head == NULL) {
		head = newNode;
		return;
	}
	
	NodePtr p, prev;
	for (p = head, prev = NULL; p != NULL; prev = p, p = p->next) {
		if (prev == NULL) {
			if (newNode->id <= p->id) {
				newNode->next = p;
				head = newNode;
				return;
			}
			else continue;
		}
		else if (prev->id <= newNode->id && newNode->id <= p->id) {
			prev->next = newNode;
			newNode->next = p;
			return;
		}
	}
	prev->next = newNode;
	
	return;
}

void main() {
	int id;
	for (;;) {
		printf("Enter id: ");
		scanf("%d", &id);
		if (id < 0) {
			break;
		}
		record_entry(id);
	}
	print_report();
}
