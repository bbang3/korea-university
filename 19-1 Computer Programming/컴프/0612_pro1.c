#include <stdio.h>
#include <stdlib.h>
typedef struct _node 
{
	char alph;
	struct Node* next;
}Node;

void print(Node *head) 
{
	printf("The list is:\n");
	for (Node *p = head; p != NULL; p = p->next) 
	{
		printf("%c --> ", p->alph);
	}
	printf("NULL\n");
}

void insert(Node **head, char alph)
{
	for (Node *p = *head; p != NULL; p = p->next)
	{
		if (p->alph == alph)
		{
			printf("Alread exist\n");
			return;
		}
	}

	Node *newNode = (Node*)malloc(sizeof(Node));
	newNode->alph = alph;
	newNode->next = NULL;

	if (*head == NULL) 
	{
		*head = newNode;
		return;
	}

	Node *p, *prev;
	for (p = *head, prev = NULL; p != NULL; prev = p, p = p->next) 
	{
		if (prev == NULL) 
		{
			if (newNode->alph < p->alph) 
			{
				newNode->next = p;
				*head = newNode;
				return;
			}
			else continue;
		}
		else if (prev->alph < newNode->alph && newNode->alph < p->alph) 
		{
			prev->next = newNode;
			newNode->next = p;
			return;
		}
	}
	prev->next = newNode;
	return;
}

void delete(Node *head, char alph)
{
	Node *prev, *p;
	for (p = head, prev = NULL; p != NULL; prev = p, p = p->next)
	{
		if (p->alph == alph)
		{
			prev->next = p;
			free(p);
			return;
		}
	}
	printf("Does not exist\n");

	return;
}

void menu()
{
	printf("1 : insert\n");
	printf("2 : delete\n");
	printf("3 : end\n");

	return;
}

int main() 
{
	Node *head = NULL;
	int choice = -1;
	char item;

	while (choice != 3)
	{
		menu();
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			printf("Enter a character: ");
			scanf(" %c", &item);

			insert(&head, item);
			print(head);
			break;
		case 2:
			printf("Enter a character to be deleted: ");
			scanf(" %c", &item);
			delete(head, item);
			break;
		default:
			printf("Invalid Input\n");
			break;
		}
	}

	return 0;
}
