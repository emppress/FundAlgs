#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    FILE_OPEN_ERROR,
    MISSING
} status;

typedef struct String
{
    char *arr;
    size_t capacity;
    size_t len;
} String;

typedef struct Node
{
    String data;
    size_t count_repeats;
    struct Node *left, *right;
} Node;

typedef struct Tree
{
    Node *root;
    int (*comparator)(const String *, const String *);
} Tree;

typedef struct Lnode
{
    Node *data;
    struct Lnode *next;
} Lnode;

typedef struct List
{
    Lnode *head;
    size_t size;
} List;

status init_tree(Tree *tree);
status delete_tree(Tree *tree);
status build_tree_from_file(Tree *tree, const char *separators, FILE *input);
status find_string(const String *str, Tree *tree, Node **found);
status find_longest_word(Tree *tree, String *str);
status find_shortest_word(Tree *tree, String *str);
int max_depth(Node *root);
status print_n_most_frequent_words(Tree *tree, int n);
status upload_tree_to_file(Tree *tree, FILE *file, char separator);
status list_destroy(List *list);
void print_menu();
void free_buf();
status string_scan(String *str);
status delete_string_content(String *string);
#endif