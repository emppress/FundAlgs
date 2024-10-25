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
    OVERFLOW_ERROR
} status;

typedef struct Node
{
    char data;
    struct Node *son, *brother, *prev;
} Node;

status build_tree(FILE *input, FILE *output);
void init_tree(Node *root);
void print_tree(Node *root, int level, FILE *output);
void delete_tree(Node *root);
status validate_input(int argc, char **argv);

#endif