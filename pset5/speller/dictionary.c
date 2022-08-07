#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//we choose 1000 as during testing it shows good performance
const unsigned long N = 1000;

node *table[1000];

//global count is simplification for this particular job and also it gives very good performance
int count = 0;

bool check(const char *word)
{
    char string[LENGTH + 1];

    //convert string to lower case
    int len = strlen(word);
    for (int i = 0; i < len; i++)
    {
        string[i] = tolower(word[i]);
    }

    //get the pointer based on the hash of the string
    node *tmp = table[hash(string)];

    //iterate through the entires and compare each word, then return true if success
    while (tmp)
    {
        if (!strcasecmp(word, tmp->word))
        {
            return true;
        }
        tmp = tmp->next;
    }

    //return false if word is not found
    return false;
}

//industry standard hashing function which is supposed to be reliable and collision free as well very fast
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    int c = *word;

    while (c == *word++)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % N;
}

bool load(const char *dictionary)
{
    unsigned int h;
    char word[LENGTH + 1];

    FILE *dict = fopen(dictionary, "r");
    if (!dict)
    {
        return false;
    }

    //load dictionary entries
    while (fscanf(dict, "%s", word) != EOF)
    {
        h = hash(word);

        node *n = calloc(sizeof(node), 1);
        strcpy(n->word, word);

        n->next = table[h];
        table[h] = n;

        count++;
    }
    fclose(dict);
    return true;
}

unsigned int size(void)
{
    return count;
}

// free the entries in the loop
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *pointer = table[i];
        while (pointer)
        {
            node *temp = pointer;
            pointer = pointer->next;
            free(temp);
        }
    }
    return true;
}
