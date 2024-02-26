#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h>	// time

#define MAX_ELEM 20
typedef struct
{
	int *heapArr;
	int last;	  // index of last element. initial value = -1
	int capacity; // 20
} HEAP;

HEAP *heapCreate(int capacity);
void heapDestroy(HEAP *heap);
int heapInsert(HEAP *heap, int data);
static void _reheapUp(HEAP *heap, int index);
int heapDelete(HEAP *heap, int *data);
static void _reheapDown(HEAP *heap, int index);
void heapPrint(HEAP *heap);

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heapCreate(int capacity)
{
	HEAP *heap = (HEAP *)malloc(sizeof(HEAP));
	if (!heap)
		return NULL;

	heap->heapArr = (int *)malloc(sizeof(int) * capacity);
	if (!heap->heapArr)
		return NULL;

	heap->last = -1;
	heap->capacity = capacity;
}

/* Free memory for heap
*/
void heapDestroy(HEAP *heap)
{
	free(heap->heapArr);
	free(heap);
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heapInsert(HEAP *heap, int data)
{
	if (heap->last == heap->capacity - 1)
		return 0;

	heap->heapArr[++heap->last] = data;
	_reheapUp(heap, heap->last);
	return 1;
}

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp(HEAP *heap, int index)
{
	if (index == 0)
		return;

	int par = (index - 1) / 2;
	if (heap->heapArr[index] > heap->heapArr[par])
	{
		int tmp = heap->heapArr[index];
		heap->heapArr[index] = heap->heapArr[par];
		heap->heapArr[par] = tmp;
		_reheapUp(heap, par);
	}
}

/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heapDelete(HEAP *heap, int *data)
{
	if (heap->last == -1)
		return 0;

	*data = heap->heapArr[0];
	heap->heapArr[0] = heap->heapArr[heap->last];
	heap->heapArr[heap->last] = *data;

	heap->last--;
	_reheapDown(heap, 0);
	return 1;
}

/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown(HEAP *heap, int index) // index: 0
{
	int largest = index, left = index * 2 + 1, right = index * 2 + 2;
	if (left <= heap->last && heap->heapArr[left] > heap->heapArr[largest])
		largest = left;
	if (right <= heap->last && heap->heapArr[right] > heap->heapArr[largest])
		largest = right;

	if (largest != index)
	{
		int tmp = heap->heapArr[index];
		heap->heapArr[index] = heap->heapArr[largest];
		heap->heapArr[largest] = tmp;

		_reheapDown(heap, largest);
	}
}

/* Print heap array */
void heapPrint(HEAP *heap)
{
	for (int i = 0; i <= heap->last; i++)
	{
		printf("%6d", heap->heapArr[i]);
	}
	printf("\n");
}

int main(void)
{
	HEAP *heap;
	int data;
	int i;

	heap = heapCreate(MAX_ELEM);

	srand(time(NULL));
	
	for (i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number

		fprintf(stdout, "Inserting %d: ", data);

		// insert function call
		heapInsert(heap, data);

		heapPrint(heap);
	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete(heap, &data);

		printf("Deleting %d: ", data);

		heapPrint(heap);
	}
	heapDestroy(heap);

	return 0;
}
