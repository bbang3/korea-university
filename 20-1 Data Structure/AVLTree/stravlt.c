#define SHOW_STEP 0
#define BALANCING 1

#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h>	// time
#include <string.h> //strcmp, strdup

#define max(x, y) (((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char *data;
	struct node *left;
	struct node *right;
	int height;
} NODE;

typedef struct
{
	NODE *root;
	int count; // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
AVL_TREE *AVL_Create(void);
void AVL_Destroy(AVL_TREE *pTree);
static void _destroy(NODE *root);
int AVL_Insert(AVL_TREE *pTree, char *data);
static NODE *_insert(NODE *root, NODE *newPtr);
static NODE *_makeNode(char *data);
char *AVL_Retrieve(AVL_TREE *pTree, char *key);
static NODE *_retrieve(NODE *root, char *key);
void AVL_Traverse(AVL_TREE *pTree);
static void _traverse(NODE *root);
void printTree(AVL_TREE *pTree);
static void _infix_print(NODE *root, int level);
static int getHeight(NODE *root);
static NODE *rotateRight(NODE *root);
static NODE *rotateLeft(NODE *root);

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create(void)
{
	AVL_TREE *tree = (AVL_TREE *)malloc(sizeof(AVL_TREE));
	if (!tree)
		return NULL;

	tree->root = NULL;
	tree->count = 0;

	return tree;
}

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy(AVL_TREE *pTree)
{
	if (pTree)
		_destroy(pTree->root);
	free(pTree);
}

static void _destroy(NODE *root)
{
	if (!root)
		return;

	_destroy(root->left);
	_destroy(root->right);

	free(root->data);
	free(root);
}
/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert(AVL_TREE *pTree, char *data)
{
	NODE *newNode = _makeNode(data);
	if (!newNode)
		return 0;

	pTree->root = _insert(pTree->root, newNode);
	pTree->count++;
	return 1;
}

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert(NODE *root, NODE *newPtr)
{
	if (!root)
	{
		root = newPtr;
		return root;
	}

	int cmp = strcmp(newPtr->data, root->data);
	if (cmp < 0)
	{
		root->left = _insert(root->left, newPtr);
#if BALANCING
		int lHeight = getHeight(root->left), rHeight = getHeight(root->right);
		if (lHeight > rHeight + 1)
		{
			int llHeight = getHeight(root->left->left), lrHeight = getHeight(root->left->right);
			if (llHeight > lrHeight) // Left of Left
			{
				root = rotateRight(root);
			}
			else if (llHeight < lrHeight) // Right of Left
			{
				root->left = rotateLeft(root->left);
				root = rotateRight(root);
			}
		}
#endif
	}
	else
	{
		root->right = _insert(root->right, newPtr);
#if BALANCING
		int lHeight = getHeight(root->left), rHeight = getHeight(root->right);

		if (rHeight > lHeight + 1)
		{
			int rlHeight = getHeight(root->right->left), rrHeight = getHeight(root->right->right);
			if (rlHeight < rrHeight) // Right of Right
			{
				root = rotateLeft(root);
			}
			else if (rlHeight > rrHeight) // Left of Right
			{
				root->right = rotateRight(root->right);
				root = rotateLeft(root);
			}
		}
#endif
	}
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	return root;
}

static NODE *_makeNode(char *data)
{
	NODE *newNode = (NODE *)malloc(sizeof(NODE));
	if (!newNode)
		return NULL;

	newNode->data = strdup(data);
	newNode->left = newNode->right = NULL;
	newNode->height = 1;

	return newNode;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve(AVL_TREE *pTree, char *key)
{
	NODE *target = _retrieve(pTree->root, key);
	if (!target)
		return NULL;

	return target->data;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve(NODE *root, char *key)
{
	if (!root)
		return NULL;

	int cmp = strcmp(key, root->data);
	if (cmp == 0)
		return root;
	else if (cmp < 0)
		return _retrieve(root->left, key);
	else
		return _retrieve(root->right, key);
}

/* Prints tree using inorder traversal
*/
void AVL_Traverse(AVL_TREE *pTree)
{
	_traverse(pTree->root);
}
static void _traverse(NODE *root)
{
	if (!root)
		return;

	_traverse(root->left);
	printf("%s ", root->data);
	_traverse(root->right);
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree(AVL_TREE *pTree)
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
	printf("%s\t\n", root->data);
	_infix_print(root->left, level + 1);
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE *root)
{
	if (!root)
		return 0;
	return root->height;
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight(NODE *root)
{
	NODE *lChild = root->left;
	if (!lChild)
		return root;

	root->left = lChild->right;
	lChild->right = root;

	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	lChild->height = max(getHeight(lChild->left), getHeight(lChild->right)) + 1;
	return lChild;
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft(NODE *root)
{
	NODE *rChild = root->right;
	if (!rChild)
		return root;

	root->right = rChild->left;
	rChild->left = root;

	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	rChild->height = max(getHeight(rChild->left), getHeight(rChild->right)) + 1;
	return rChild;
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}

	// creates a null tree
	tree = AVL_Create();

	if (!tree)
	{
		fprintf(stderr, "Cannot create tree!\n");
		return 100;
	}

	FILE *fp = fopen(argv[1], "rt");
	if (fp == NULL)
	{
		fprintf(stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while (fscanf(fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf(stdout, "Insert %s>\n", str);
#endif
		// insert function call
		AVL_Insert(tree, str);

#if SHOW_STEP
		fprintf(stdout, "Tree representation:\n");
		printTree(tree);
#endif
	}

	fclose(fp);

#if SHOW_STEP
	fprintf(stdout, "\n");

	// inorder traversal
	fprintf(stdout, "Inorder traversal: ");
	AVL_Traverse(tree);
	fprintf(stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf(stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf(stdout, "Height of tree: %d\n", tree->root->height);
	fprintf(stdout, "# of nodes: %d\n", tree->count);

	// retrieval
	char *key;
	fprintf(stdout, "Query: ");
	while (fscanf(stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve(tree, str);

		if (key)
			fprintf(stdout, "%s found!\n", key);
		else
			fprintf(stdout, "%s NOT found!\n", str);

		fprintf(stdout, "Query: ");
	}

	// destroy tree
	AVL_Destroy(tree);

	return 0;
}
