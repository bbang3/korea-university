#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define WORDLENGTH 100
#define INIT_CAPACITY 100000
#define MAXRANK 10

#define min(a, b) (a) < (b) ? (a) : (b)

#define INSERT_COST 1
#define DELETE_COST 1
#define SUB_COST 1
#define TRANS_COST 1

typedef struct _tnode
{
    char word[100];
    int word_idx;
    int dist;

    struct _tnode* prev;
    struct _tnode* next;

}tWord_node;

typedef struct
{
    int count;
    tWord_node* head;
    tWord_node* tail;
}tWord_list;

tWord_node* create_node(char* word, int word_idx, int dist)
{
    tWord_node* node = (tWord_node*)malloc(sizeof(tWord_node));

    node->prev = node->next = NULL;
    if(word != NULL)
        strcpy(node->word, word);
    
    node->word_idx = word_idx;
    node->dist = dist;

    return node;
}

tWord_list* create_wlist()
{
    tWord_list* wlist = (tWord_list*)malloc(sizeof(tWord_list));
    tWord_node *head, *tail;

    wlist->head = create_node(NULL, -1, -1); // dummy
    wlist->tail = create_node(NULL, -1, 1000000000);
    wlist->head->next = wlist->tail;
    wlist->tail->prev = wlist->head;
    
    wlist->count = 0;

    return wlist;
}

void destroy_wlist(tWord_list *wlist)
{
    tWord_node *cur = wlist->head;
    while(cur != NULL)
    {
        tWord_node *del = cur;
        cur = cur->next;
        free(del);
    }

    free(wlist);
}

int search_wlist(tWord_list* wlist, int word_idx)
{
    tWord_node* cur = wlist->head->next;

    while (cur->next != NULL)
    {
        if (cur->word_idx == word_idx)
            return 1;
        cur = cur->next;
    }

    return 0;
}

void print_wlist(tWord_list* wlist)
{
    int rank = 1;
    tWord_node* cur = wlist->head->next;
    
    while (cur->next != NULL)
    {
        printf("%d: %s %d\n", rank, cur->word, cur->dist);
        cur = cur->next;
        rank++;
    }
}

void delete_tail(tWord_list *wlist)
{
    tWord_node* del_node = wlist->tail->prev;

    del_node->prev->next = del_node->next;
    del_node->next->prev = del_node->prev;

    free(del_node);
    wlist->count--;
}

void add_node(tWord_list* wlist, char* word, int word_idx, int dist)
{
    tWord_node* cur = wlist->head->next;

    int rank = 1;
    while (cur->next != NULL && cur->dist <= dist)
    {
        cur = cur->next;
        rank++;
    }
    if(rank > MAXRANK)
        return;
        
    tWord_node* new_node = create_node(word, word_idx, dist);

    new_node->prev = cur->prev;
    new_node->next = cur;

    cur->prev->next = new_node;
    cur->prev = new_node;

    wlist->count++;

    if(wlist->count > MAXRANK)
        delete_tail(wlist);
}

int min_editdistance(char* str1, char* str2)
{
    int dist[WORDLENGTH][WORDLENGTH] = { 0 };

    int n = strlen(str1), m = strlen(str2);
    for (int i = 1; i <= n; i++)
        dist[i][0] = i;
    for (int j = 1; j <= m; j++)
        dist[0][j] = j;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
        {
            if (str1[i - 1] == str2[j - 1])
            {
                dist[i][j] = dist[i - 1][j - 1];
            }
            else
            {
                dist[i][j] = min(dist[i - 1][j] + INSERT_COST, dist[i][j - 1] + DELETE_COST);
                dist[i][j] = min(dist[i][j], dist[i - 1][j - 1] + SUB_COST);

                if (i >= 2 && j >= 2 &&
                    str1[i - 1] == str2[j - 2] && str1[i - 2] == str2[j - 1])
                    dist[i][j] = min(dist[i][j], dist[i - 2][j - 2] + TRANS_COST);
            }
        }
    return dist[n][m];
}

void search_ngram(FILE* fp_ngram, char* input, tWord_list* wlist)
{
    char dict_word[WORDLENGTH] = { 0 };
    int word_idx;

    while (fscanf(fp_ngram, "%s %d\n", dict_word, &word_idx) != EOF)
    {
        if (search_wlist(wlist, word_idx) == 0)
        {
            int dist = min_editdistance(input, dict_word);
            add_node(wlist, dict_word, word_idx, dist);
        }
    }
}

void search(FILE *fp_ngram[26][26], FILE *fp_ngram_small, char *input, tWord_list *wlist)
{
    int len = strlen(input);
    int occurence[26][26] = {0};

    if (len == 1)
    {
        fprintf(stderr, "Seraching small.ngram\n");
        search_ngram(fp_ngram_small, input, wlist);
        return;
    }

    for (int i = 0; i < len - 1; i++)
    {
        char c1 = input[i], c2 = input[i + 1];
        occurence[c1 - 'a'][c2 - 'a'] = 1;
    }

    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
        {
            if (occurence[i][j] > 0)
            {
                fprintf(stderr, "Searching %c%c.ngram\n", i + 'a', j + 'a');
                search_ngram(fp_ngram[i][j], input, wlist);
            }
        }
}

void fopen_ngram(char* dirname, FILE* fp_ngram[26][26], FILE **fp_ngram_small)
{
    char full_path[100] = { 0 };
    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
        {
            char filename[] = "/__.ngram";
            filename[1] = i + 'a';
            filename[2] = j + 'a';
            strcpy(full_path, dirname);
            strcat(full_path, filename);

            fp_ngram[i][j] = fopen(full_path, "rt");
        }
    
    strcpy(full_path, dirname);
    strcat(full_path, "/small.ngram");
    *fp_ngram_small = fopen(full_path, "rt");
}

int main()
{
    FILE* fp_ngram_small;
    FILE* fp_ngram[26][26];
    char dirname[] = "ngram_index";

    fopen_ngram(dirname, fp_ngram, &fp_ngram_small);
    fprintf(stderr, "ngram file loaded\n");

    tWord_list* wlist = create_wlist();

    char input_str[WORDLENGTH] = { 0 };
    int len;

    printf("Search: ");
    scanf("%s", input_str);
    len = strlen(input_str);

    fprintf(stderr, "Searching words from ngram...\n");
    search(fp_ngram, fp_ngram_small, input_str, wlist);

    fprintf(stderr, "========================================\n");
    fprintf(stderr, "approximate words list\n");
    print_wlist(wlist);

    destroy_wlist(wlist);

    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
            fclose(fp_ngram[i][j]);
    
    fclose(fp_ngram_small);

    return 0;
}