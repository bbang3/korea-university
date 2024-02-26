#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h>	// toupper

#define QUIT 1
#define FORWARD_PRINT 2
#define BACKWARD_PRINT 3
#define SEARCH 4
#define DELETE 5
#define COUNT 6

// User structure type definition
typedef struct
{
	char* token;
	int freq;
} tTOKEN;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tTOKEN* dataPtr;
	struct node* llink;
	struct node* rlink;
} NODE;

typedef struct
{
	int count;
	NODE* pos; // unused
	NODE* head;
	NODE* rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations
int searchList(LIST* pList, char* pArgu, tTOKEN** pDataOut);
int countList(LIST* pList);
int emptyList(LIST* pList);
void printList(LIST* pList);
void printListR(LIST* pList);
static int _insert(LIST* pList, NODE* pPre, tTOKEN* dataInPtr);
static void _delete(LIST* pList, NODE* pPre, NODE* pLoc, tTOKEN** dataOutPtr);
static int _search(LIST* pList, NODE** pPre, NODE** pLoc, char* pArgu);
tTOKEN* createToken(char* str);
void destroyToken(tTOKEN* pToken);
int get_action();

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST* createList(void)
{
	LIST* pList = (LIST*)malloc(sizeof(LIST));
	if (!pList)
		return NULL;

	pList->head = pList->rear = pList->pos = NULL;
	pList->count = 0;

	return pList;
}

/* Deletes all data in list and recycles memory
*/
void destroyList(LIST* pList)
{
	while (pList->head != NULL)
	{
		NODE* delNode = pList->head;
		pList->head = pList->head->rlink;
		pList->count--;

		destroyToken(delNode->dataPtr);
		free(delNode);
	}
	free(pList);
}

/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode(LIST* pList, tTOKEN* dataInPtr)
{
	NODE* pPre = NULL, * pLoc = NULL;
	int isFound = _search(pList, &pPre, &pLoc, dataInPtr->token);
	if (isFound)
	{
		pLoc->dataPtr->freq++;
		return 2;
	}

	int isInserted = _insert(pList, pPre, dataInPtr);
	if (!isInserted)
		return 0;

	pList->count++;
	return 1;
}

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode(LIST* pList, char* keyPtr, tTOKEN** dataOut)
{
	NODE* pPre = NULL, * pLoc = NULL;
	int isFound = _search(pList, &pPre, &pLoc, keyPtr);
	if (!isFound)
		return 0;

	_delete(pList, pPre, pLoc, dataOut);
	pList->count--;
	return 1;
}

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/

int searchList(LIST* pList, char* pArgu, tTOKEN** pDataOut)
{
	NODE* pPre = NULL, * pLoc = NULL;
	int found = _search(pList, &pPre, &pLoc, pArgu);
	if (!found)
		return 0;

	*pDataOut = pLoc->dataPtr;
	return 1;
}

/* returns number of nodes in list
*/
int countList(LIST* pList)
{
	return pList->count;
}

/* returns	1 empty
			0 list has data
*/
int emptyList(LIST* pList)
{
	return pList->count == 0;
}

//int fullList(LIST *pList);

/* prints data from list (forward)
*/
void printList(LIST* pList)
{
	NODE* cur = pList->head;
	while (cur != NULL)
	{
		printf("%s\t%d\n", cur->dataPtr->token, cur->dataPtr->freq);
		cur = cur->rlink;
	}
}

/* prints data from list (backward)
*/
void printListR(LIST* pList)
{
	NODE* cur = pList->rear;
	while (cur != NULL)
	{
		printf("%s\t%d\n", cur->dataPtr->token, cur->dataPtr->freq);
		cur = cur->llink;
	}
}

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert(LIST* pList, NODE* pPre, tTOKEN* dataInPtr)
{
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	if (!newNode)
		return 0;

	newNode->llink = newNode->rlink = NULL;
	newNode->dataPtr = dataInPtr;

	if (emptyList(pList)) // add into empty list
	{
		pList->head = pList->rear = newNode;
	}
	else if (pPre == NULL) // add into head
	{
		newNode->rlink = pList->head;
		pList->head->llink = newNode;
		pList->head = newNode;
	}
	else if (pPre->rlink == NULL) // add into rear
	{
		newNode->llink = pPre;
		pPre->rlink = newNode;
		pList->rear = newNode;
	}
	else
	{
		newNode->llink = pPre;
		newNode->rlink = pPre->rlink;

		pPre->rlink->llink = newNode;
		pPre->rlink = newNode;
	}
	return 1;
}

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete(LIST* pList, NODE* pPre, NODE* pLoc, tTOKEN** dataOutPtr)
{
	if (countList(pList) == 1) // delete head and rear
	{
		pList->head = pList->rear = NULL;
	}
	else if (pPre == NULL) // delete head
	{
		pLoc->rlink->llink = NULL;
		pList->head = pList->head->rlink;
	}
	else if (pLoc->rlink == NULL) // delete rear
	{
		pPre->rlink = NULL;
		pList->rear = pList->rear->llink;
	}
	else
	{
		pPre->rlink = pLoc->rlink;
		pLoc->rlink->llink = pPre;
	}

	*dataOutPtr = pLoc->dataPtr;
	free(pLoc);
}

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search(LIST* pList, NODE** pPre, NODE** pLoc, char* pArgu)
{
	*pPre = NULL;
	*pLoc = pList->head;
	int cmp = -1;
	while (*pLoc != NULL && (cmp = strcmp(pArgu, (*pLoc)->dataPtr->token)) > 0)
	{
		*pPre = *pLoc;
		*pLoc = (*pLoc)->rlink;
	}
	if (cmp == 0) return 1;
	return 0;
}

/* Allocates dynamic memory for a token structure, initialize fields(token, freq) and returns its address to caller
	return	token structure pointer
			NULL if overflow
*/
tTOKEN* createToken(char* str)
{
	tTOKEN* newToken = (tTOKEN*)malloc(sizeof(tTOKEN));
	if (!newToken) // overflow
		return NULL;

	newToken->token = strdup(str);
	newToken->freq = 1;

	return newToken;
}

/* Deletes all data in token structure and recycles memory
*/
void destroyToken(tTOKEN* pToken)
{
	free(pToken->token);
	free(pToken);
}

////////////////////////////////////////////////////////////////////////////////
/* gets user's input
*/
int get_action()
{
	char ch;
	scanf("%c", &ch);
	ch = toupper(ch);
	switch (ch)
	{
	case 'Q':
		return QUIT;
	case 'F':
		return FORWARD_PRINT;
	case 'B':
		return BACKWARD_PRINT;
	case 'S':
		return SEARCH;
	case 'D':
		return DELETE;
	case 'C':
		return COUNT;
	}
	return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	LIST* list;
	char str[1024];
	tTOKEN* pToken;
	int ret;
	FILE* fp;

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}

	fp = fopen(argv[1], "rt");
	if (!fp)
	{
		fprintf(stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}

	// creates an empty list
	list = createList();
	if (!list)
	{
		printf("Cannot create list\n");
		return 100;
	}

	while (fscanf(fp, "%s", str) == 1)
	{
		pToken = createToken(str);

		// insert function call
		ret = addNode(list, pToken); // strdup(str)

		if (ret == 2) // duplicated
			destroyToken(pToken);
	}

	fclose(fp);

	fprintf(stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");

	while (1)
	{
		int action = get_action();
		switch (action)
		{
		case QUIT:
			destroyList(list);
			return 0;

		case FORWARD_PRINT:
			printList(list);
			break;

		case BACKWARD_PRINT:
			printListR(list);
			break;

		case SEARCH:
			fprintf(stderr, "Input a string to find: ");
			fscanf(stdin, "%s", str);

			if (searchList(list, str, &pToken))
				fprintf(stdout, "(%s, %d)\n", pToken->token, pToken->freq);
			else
				fprintf(stdout, "%s not found\n", str);

			break;

		case DELETE:
			fprintf(stderr, "Input a string to delete: ");
			fscanf(stdin, "%s", str);

			if (removeNode(list, str, &pToken))
			{
				fprintf(stdout, "%s deleted\n", pToken->token);
				destroyToken(pToken);
			}
			else
				fprintf(stdout, "%s not found\n", str);
			break;

		case COUNT:
			fprintf(stdout, "%d\n", countList(list));
			break;
		}
		if (action)
			fprintf(stderr, "Select Q)uit, F)orward print, B)ackward print, S)earch, D)elete, C)ount: ");
	}
	return 0;
}
