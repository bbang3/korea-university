#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

void index_word(FILE *fp_ngram[26][26], char word[], int len, int word_idx)
{
    int occurence[26][26] = {0};
    if (len == 1)
    {
        char c1 = word[0];
        for (int i = 0; i < 26; i++)
            occurence[c1 - 'a'][i] = occurence[i][c1 - 'a'] = 1;
    }
    else
    {
        for (int i = 0; i < len - 1; i++)
        {
            char c1 = word[i], c2 = word[i + 1];
            occurence[c1 - 'a'][c2 - 'a'] = 1;
        }
    }

    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
        {
            if(occurence[i][j] > 0)
            {
                fprintf(fp_ngram[i][j], "%s %d\n", word, word_idx);
            }
        }
}

void fopen_ngram(char *dirname, FILE *fp_ngram[26][26], FILE **fp_ngram_small)
{
    char full_path[100] = {0};
    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
        {
            char filename[] = "/__.ngram";
            filename[1] = i + 'a';
            filename[2] = j + 'a';
            strcpy(full_path, dirname);
            strcat(full_path, filename);

            fp_ngram[i][j] = fopen(full_path, "wt");
        }

    char filename[] = "/small.ngram";
    strcpy(full_path, dirname);
    strcat(full_path, filename);
    *fp_ngram_small = fopen(full_path, "wt");
}

int main()
{
    FILE *fp_dict = fopen("words.txt", "rt");
    FILE *fp_ngram[26][26];
    FILE *fp_ngram_small;
    
    char *dirname = "ngram_index";
    mkdir(dirname, 0777);
    fprintf(stderr, "make indexing folder\n");
    
    fopen_ngram(dirname, fp_ngram, &fp_ngram_small);
    fprintf(stderr, "fopen ngram files\n");
    
    int num_word = 0, sum_wlen = 0;
    char str[100] = {0};
    while (fscanf(fp_dict, "%s\n", str) != EOF)
    {
        int len = strlen(str);
        sum_wlen += len;
        
        index_word(fp_ngram, str, len, num_word);
        if(len <= 2) fprintf(fp_ngram_small, "%s %d\n", str, num_word);

        if(num_word % 100000 == 0)
            fprintf(stderr, "%d words processed\n", num_word);
        num_word++;
    }

    fprintf(stderr, "num of words: %d\n", num_word);
    fprintf(stderr, "average word length: %lf\n", sum_wlen / (double)num_word);

    fclose(fp_dict);
    fclose(fp_ngram_small);
    for (int i = 0; i < 26; i++)
        for (int j = 0; j < 26; j++)
            fclose(fp_ngram[i][j]);

    return 0;
}