#include <stdio.h>
#include <stdlib.h>    // malloc
#include <string.h>    // strdup
#include <ctype.h>    // isupper, tolower

#define INSERT_COST    1
#define DELETE_COST    1
#define SUBSTITUTE_COST    1
#define TRANSPOSE_COST    1

typedef struct node {
    char data[100];
    int distance;

    struct node* nxt;

} NODE;

typedef struct
{
    int count;
    NODE* head;
} LIST;


void printIndex(NODE *head);
NODE* createNode(char* data, int distance);

LIST* createList() {
    LIST* list = (LIST*)malloc(sizeof(LIST));
    
    list->head = NULL;
    list->count = 0;

    return list;
}

NODE* createNode(char* data, int distance) {

    NODE* node = (NODE*)malloc(sizeof(NODE));

    node->nxt = NULL;
    node->distance = distance;
    strcpy(node->data, data);

    return node;
}


static int __GetMin3(int a, int b, int c)
{
    int min = a;
    if (b < min)
        min = b;
    if (c < min)
        min = c;
    return min;
}
static int __GetMin4(int a, int b, int c, int d)
{
    int min = __GetMin3(a, b, c);
    return (min > d) ? d : min;
}
int min_editdistance(char* str1, char* str2) {
    int n = strlen(str1);
    int m = strlen(str2);

    int i, j;
    int d[n + 1][m + 1];

    memset(d, 0, sizeof(d));


    for (i = 1; i <= n; i++) {
        d[i][0] = i;
    }

    for (int j = 0; j <= m; j++) {
        d[0][j] = j;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int tmp = (str1[i - 1] == str2[j - 1]) ? 0 : SUBSTITUTE_COST;
            if ((i > 1 && j > 1) && (str1[i - 2] == str2[j - 1] && str1[i - 1] == str2[j - 2]))
            {
                d[i][j] = __GetMin4(d[i - 1][j] + DELETE_COST, d[i][j - 1] + INSERT_COST, d[i - 1][j - 1] + tmp, d[i - 2][j - 2] + TRANSPOSE_COST);
            }
            else
            {
                d[i][j] = __GetMin3(d[i - 1][j] + DELETE_COST, d[i][j - 1] + INSERT_COST, d[i - 1][j - 1] + tmp);
            }
        }
    }
    return d[n][m];
}

int checkinlist(LIST* list, char* data) {
    NODE* cur = list->head;

    while (cur != NULL) {
        if (!strcmp(cur->data, data)) {
            return 1;
        }
        cur = cur->nxt;
    }
    return 0;
}

void deleteNode(LIST* list) {
    NODE* delNode = list->head;
    list->head = list->head->nxt;

    free(delNode);
    list->count--;
}

void checkNode(LIST* list, char* data, int distance) {
    NODE* cur = list->head;
    NODE* pre = NULL;
    if (cur != NULL && cur->distance <= distance)
        return;

    while (cur != NULL && distance < cur->distance) {
        pre = cur;
        cur = cur->nxt;
    }
    NODE* new_node = createNode(data, distance);

    if (list->head == NULL) {
        list->head = new_node;
    }
    else if (pre == NULL) {
        cur->nxt = list->head;
        list->head = cur;
    }
    else if (cur == NULL) {
        pre->nxt = new_node;
    }
    else {
        pre->nxt = new_node;
        new_node->nxt = cur;
    }

    list->count++;
    if (list->count > 10) {
        deleteNode(list);
    }

    // printIndex(list->head);
    // puts("================");
}

void searchMain(FILE* fp, char* look_for, LIST* list) {
    char word[100];
    memset(word, 0, sizeof(word));
    while (fscanf(fp, "%s", word) != EOF) {
        if (!checkinlist(list, word)) {
            int distance = min_editdistance(look_for, word);
            checkNode(list, word, distance);
        }
    }
}



void search(FILE* fp[26 * 26], char* look_for, LIST* list) {
    int length = strlen(look_for);

    int check[26 * 26];

    memset(check, 0, sizeof(check));

    if (length == 1) {
        for (int i = 0; i < 26; i++) {
            char fst = look_for[0] - 'a';
            char scnd = i;
            check[fst * 26 + scnd]++;
        }

        for (int i = 0; i < 26; i++) {
            char fst = i;
            char scnd = look_for[0] - 'a';
            check[fst * 26 + scnd]++;
        }


        for (int i = 0; i < 26 * 26; i++) {
            if (check[i] > 0) {
                //fprintf(stderr, "Searching %c%c.txt\n", 'a' + (i / 26), 'a' + (i % 26));
                searchMain(fp[i], look_for, list);
            }
        }
    }
    else {
        for (int i = 0; i < (length - 1); i++) {
            char fst = look_for[i] - 'a';
            char scnd = look_for[i + 1] - 'a';
            check[fst * 26 + scnd]++;
        }

        for (int i = 0; i < 26 * 26; i++) {
            if (check[i] > 0) {
                // fprintf(stderr, "Searching %c%c.txt\n", 'a' + (i / 26), 'a' + (i % 26));
                searchMain(fp[i], look_for, list);
            }
        }
    }
}

void printIndex(NODE* cur) {
    if (cur == NULL) return;

    printIndex(cur->nxt);
    printf("%s %d\n", cur->data, cur->distance);
}

void destoryList(LIST *list)
{
    NODE *cur = list->head;
    NODE *pre = NULL;
    while (cur){
        // free(cur->data);
        pre = cur;
        cur = cur->nxt;
        free(pre);
    }
    free(list);
}

void fileOpen(char* idx_name, FILE* fp[26 * 26]) {
    char dir_path[100];
    memset(dir_path, 0, sizeof(dir_path));
    char index[3];
    for (int i = 0; i < 26 * 26; i++) {
        char f_name[8];
        memset(f_name, 0, sizeof(f_name));
        strcat(f_name, "/");
        index[0] = 'a' + (i / 26);
        index[1] = 'a' + (i % 26);
        index[2] = 0;
        strcat(f_name, index);
        strcat(f_name, ".txt");

        strcpy(dir_path, idx_name);
        strcat(dir_path, f_name);

        fp[i] = fopen(dir_path, "rt");
    }
}


int main(int argc, char** argv) {

    if (argc != 1) {
        fprintf(stderr, "Incorrect number of argument");
        return 1;
    }

    FILE* fp[26 * 26];

    char idx_name[100] = "directory";

    fileOpen(idx_name, fp);
    //fprintf(stderr, "ngram file loaded\n");

    LIST* list = createList();


    char look_for[50];

    fscanf(stdin, "%s", look_for);

    //fprintf(stderr, "searching words from ngram...\n");

    search(fp, look_for, list);
    printIndex(list->head);
    destoryList(list);
    for(int i =0; i<26 * 26;i++)
       fclose(fp[i]);
    return 0;
}
