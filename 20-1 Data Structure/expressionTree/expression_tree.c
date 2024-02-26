#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h>	// isdigit
#include <assert.h> // assert

#define MAX_STACK_SIZE 50

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char data;
	struct node *left;
	struct node *right;
} NODE;

typedef struct
{
	NODE *root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *createTree(void);

/* Deletes all data in tree and recycles memory */
void destroyTree(TREE *pTree);

static void _destroy(NODE *root);
/*  Allocates dynamic memory for a node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
static NODE *_makeNode(char ch);

/* converts postfix expression to binary tree
	return	1 success
			0 invalid postfix expression
*/
int postfix2tree(char *expr, TREE *pTree);

/* Print node in tree using inorder traversal */
void traverseTree(TREE *pTree);

/* internal traversal function
	an implementation of ALGORITHM 6-6 */
static void _traverse(NODE *root);

/* Print tree using inorder right-to-left traversal */
void printTree(TREE *pTree);

/* internal traversal function */
static void _infix_print(NODE *root, int level);

/* evaluate postfix expression
	return	value of expression */
float evalPostfix(char *expr);

TREE *createTree(void)
{
	TREE *tree = (TREE*)malloc(sizeof(TREE));
	if(!tree) 
		return NULL;

	tree->root = NULL;
	return tree;
}

////////////////////////////////////////////////////////////////////////////////
void destroyTree(TREE *pTree)
{
	if (pTree)
	{
		_destroy(pTree->root);
	}

	free(pTree);
}

////////////////////////////////////////////////////////////////////////////////
void printTree(TREE *pTree)
{
	_infix_print(pTree->root, 0);
}

////////////////////////////////////////////////////////////////////////////////
void traverseTree(TREE *pTree)
{
	_traverse(pTree->root);
}

int postfix2tree(char *expr, TREE *pTree)
{
	NODE *stack[MAX_STACK_SIZE];
	int top = -1;
	int isValid = 1;

	for(int i = 0; expr[i]; i++)
	{
		if(isdigit(expr[i])) 
		{
			stack[++top] = _makeNode(expr[i]);
		}
		else if(expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/')
		{
			if(top <= 0)
			{
				isValid = 0;
				break;
			}
			else
			{
				NODE *lchild = stack[top - 1], *rchild = stack[top];
				NODE *parent = _makeNode(expr[i]);
				top -= 2;

				parent->left = lchild;
				parent->right = rchild;
				stack[++top] = parent;
			}
		}
		else
		{
			isValid = 0;
			break;
		}
		
	}

	if (top > 0)
		isValid = 0;

	if(!isValid)
	{
		for(int i = top; i >= 0; i--)
			_destroy(stack[i]);
		return 0;
	}

	pTree->root = stack[top];
	return 1;
}

float evalPostfix(char *expr)
{
	float stack[MAX_STACK_SIZE];
	int top = -1;

	for(int i = 0; expr[i]; i++)
	{
		if(isdigit(expr[i])) 
			stack[++top] = expr[i] - '0';
		else
		{
			float a = stack[top - 1], b = stack[top];
			float res;

			if(expr[i] == '+') res = a + b;
			else if(expr[i] == '-') res = a - b;
			else if(expr[i] == '*') res = a * b;
			else res = a / b;

			top -= 2;
			stack[++top] = res;
		}
	}
	return stack[top];
}

static void _destroy(NODE *root)
{
	if(!root) return;

	_destroy(root->left);
	_destroy(root->right);
	free(root);
}

static NODE *_makeNode(char ch)
{
	NODE *newNode = (NODE *)malloc(sizeof(NODE));
	if(!newNode)
		return NULL;

	newNode->data = ch;
	newNode->left = newNode->right = NULL;

	return newNode;
}

static void _traverse(NODE *root)
{
	if(!root) return;
	
	if(isdigit(root->data))
	{
		printf("%c", root->data);
	}
	else
	{
		printf("(");
		_traverse(root->left);
		printf("%c", root->data);
		_traverse(root->right);
		printf(")");
	}
}

static void _infix_print(NODE *root, int level)
{
	if (!root)
		return;

	_infix_print(root->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("\t");
	printf("%c\n", root->data);
	_infix_print(root->left, level + 1);
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TREE *tree;
	char expr[1024];

	fprintf(stdout, "\nInput an expression (postfix): ");

	while (fscanf(stdin, "%s", expr) == 1)
	{
		// creates a null tree
		tree = createTree();

		if (!tree)
		{
			printf("Cannot create tree\n");
			return 100;
		}

		// postfix expression -> expression tree
		int ret = postfix2tree( expr, tree);
		if (!ret)
		{
			fprintf(stdout, "invalid expression!\n");
			destroyTree(tree);
			continue;
		}

		// expression tree -> infix expression
		fprintf(stdout, "\nInfix expression : ");
		traverseTree(tree);

		// print tree with right-to-left infix traversal
		fprintf(stdout, "\n\nTree representation:\n");
		printTree(tree);

		// evaluate postfix expression
		float val = evalPostfix(expr);
		fprintf(stdout, "\nValue = %f\n", val);

		// destroy tree
		destroyTree(tree);

		fprintf(stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}