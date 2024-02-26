#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h>	// time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int data;
	struct node *left;
	struct node *right;
} NODE;

typedef struct
{
	NODE *root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
TREE *BST_Create(void);
void BST_Destroy(TREE *pTree);
static void _destroy(NODE *root);
int BST_Insert(TREE *pTree, int data);
static void _insert(NODE *root, NODE *newPtr);
NODE *_makeNode(int data);
int BST_Delete(TREE *pTree, int dltKey);
static NODE *_delete(NODE *root, int dltKey, int *success);
int *BST_Retrieve(TREE *pTree, int key);
static NODE *_retrieve(NODE *root, int key);
void BST_Traverse(TREE *pTree);
static void _traverse(NODE *root);
void printTree(TREE *pTree);
static void _infix_print(NODE *root, int level);
int BST_Empty(TREE *pTree);

/* Allocates dynamic memory for a tree head node and returns its address to caller
		return	head node pointer
				NULL if overflow
	*/
TREE *BST_Create(void)
{
	TREE *tree = (TREE *)malloc(sizeof(TREE));
	if (!tree)
		return NULL;

	tree->root = NULL;
	return tree;
}

/* Deletes all data in tree and recycles memory
	*/
void BST_Destroy(TREE *pTree)
{
	if (pTree)
	{
		_destroy(pTree->root);
	}

	free(pTree);
}

/* internal function (not mandatory)
	*/
static void _destroy(NODE *root)
{
	if (!root)
		return;

	_destroy(root->left);
	_destroy(root->right);
	free(root);
}

/* Inserts new data into the tree
		return	1 success
				0 overflow
	*/
int BST_Insert(TREE *pTree, int data)
{
	NODE *newNode = _makeNode(data);
	if (!newNode)
		return 0;

	if (!pTree->root)
	{
		pTree->root = newNode;
	}
	else
	{
		_insert(pTree->root, newNode);
	}
	return 1;
}

/* internal function (not mandatory)
	*/
static void _insert(NODE *root, NODE *newPtr)
{
	if (newPtr->data < root->data)
	{
		if (!root->left)
		{
			root->left = newPtr;
		}
		else
		{
			_insert(root->left, newPtr);
		}
	}
	else
	{
		if (!root->right)
		{
			root->right = newPtr;
		}
		else
		{
			_insert(root->right, newPtr);
		}
	}
}

NODE *_makeNode(int data)
{
	NODE *newNode = (NODE *)malloc(sizeof(NODE));
	if (!newNode)
		return NULL;

	newNode->data = data;
	newNode->left = newNode->right = NULL;

	return newNode;
}

/* Deletes a node with dltKey from the tree
		return	1 success
				0 not found
	*/
int BST_Delete(TREE *pTree, int dltKey)
{
	int success = 0;
	NODE *pRoot = _delete(pTree->root, dltKey, &success);
	if (!success)
		return 0;

	pTree->root = pRoot;
	return 1;
}

/* internal function
		success is 1 if deleted; 0 if not
		return	pointer to root
	*/
static NODE *_delete(NODE *root, int dltKey, int *success)
{
	if (!root)
	{
		*success = 0;
		return NULL;
	}

	if (dltKey < root->data)
	{
		root->left = _delete(root->left, dltKey, success);
	}
	else if (dltKey > root->data)
	{
		root->right = _delete(root->right, dltKey, success);
	}
	else
	{

		if (!root->left)
		{
			NODE *dltNode = root;
			root = root->right;
			free(dltNode);
		}
		else if (!root->right)
		{
			NODE *dltNode = root;
			root = root->left;
			free(dltNode);
		}
		else
		{
			NODE *smallest = root->right;
			while (smallest->left != NULL)
				smallest = smallest->left;

			// exchange root data and smallest node data
			int tmp = root->data;
			root->data = smallest->data;
			smallest->data = tmp;
			// delete smallest node from right subtree of root
			root->right = _delete(root->right, smallest->data, success);
		}
		*success = 1;
	}
	return root;
}

/* Retrieve tree for the node containing the requested key
		return	address of data of the node containing the key
				NULL not found
	*/
int *BST_Retrieve(TREE *pTree, int key)
{
	NODE *target = _retrieve(pTree->root, key);
	if (!target)
		return NULL;

	return &target->data;
}

/* internal function
		Retrieve node containing the requested key
		return	address of the node containing the key
				NULL not found
	*/
static NODE *_retrieve(NODE *root, int key)
{
	if (!root)
		return NULL;

	if (key < root->data)
	{
		return _retrieve(root->left, key);
	}
	else if (key > root->data)
	{
		return _retrieve(root->right, key);
	}
	else
	{
		return root;
	}
}

/* prints tree using inorder traversal
	*/
void BST_Traverse(TREE *pTree)
{
	_traverse(pTree->root);
}

static void _traverse(NODE *root)
{
	if (!root)
		return;

	_traverse(root->left);
	printf("%d ", root->data);
	_traverse(root->right);
}

/* Print tree using inorder right-to-left traversal
	*/
void printTree(TREE *pTree)
{
	_infix_print(pTree->root, 0);
}
/* internal traversal function
	*/
static void _infix_print(NODE *root, int level)
{
	if (!root)
		return;

	_infix_print(root->right, level + 1);
	for (int i = 0; i < level; i++)
		printf("\t");
	printf("%d\n", root->data);
	_infix_print(root->left, level + 1);
}

/*
		return 1 if the tree is empty; 0 if not
	*/
int BST_Empty(TREE *pTree)
{
	return pTree->root == NULL;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TREE *tree;
	int data;

	// creates a null tree
	tree = BST_Create();

	if (!tree)
	{
		printf("Cannot create a tree!\n");
		return 100;
	}

	fprintf(stdout, "How many numbers will you insert into a BST: ");

	int numbers;
	scanf("%d", &numbers);

	fprintf(stdout, "Inserting: ");

	srand(time(NULL));
	for (int i = 0; i < numbers; i++)
	{
		data = rand() % (numbers * 3) + 1; // random number (1 ~ numbers * 3)

		fprintf(stdout, "%d ", data);

		// insert funtion call
		int ret = BST_Insert(tree, data);
		if (!ret)
			break;
	}
	fprintf(stdout, "\n");

	// inorder traversal
	fprintf(stdout, "Inorder traversal: ");
	BST_Traverse(tree);
	fprintf(stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf(stdout, "Tree representation:\n");
	printTree(tree);

	while (1)
	{
		fprintf(stdout, "Input a number to delete: ");
		int num;
		int ret = scanf("%d", &num);
		if (ret != 1)
			break;

		ret = BST_Delete(tree, num);
		if (!ret)
		{
			fprintf(stdout, "%d not found\n", num);
			continue;
		}

		// print tree with right-to-left infix traversal
		fprintf(stdout, "Tree representation:\n");
		printTree(tree);

		if (BST_Empty(tree))
		{
			fprintf(stdout, "Empty tree!\n");
			break;
		}
	}

	BST_Destroy(tree);

	return 0;
}
