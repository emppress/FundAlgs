#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define COUNT_VECTORS 26

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    MISSING,
    FILE_OPEN_ERROR,
    DIVISION_BY_ZERO_ERROR
} status;

typedef struct String
{
    char *arr;
    size_t capacity;
    size_t len;
} String;

typedef union Value
{
    String var;
    char op;
    Tnode *ptr;
} Value;

typedef enum Type
{
    OP,
    VAR,
    NODE_PTR
} Type;

typedef struct Tnode
{
    Value value;
    Type type;
    struct Tnode l, r;
} Tnode;

typedef struct Snode
{
    Value value;
    struct Snode *next;
} Snode;

typedef struct Stack
{
    Snode *head;
    size_t size;
} Stack;

status read_string(FILE *input, String *str);
status delete_string_content(String *string);
status add_char_to_string(String *str, char ch);
status string_init(String *string);
status read_word(String *str);
status string_to_long(const char *str, long *result, int base);
status validate_file(char **argv);
#endif