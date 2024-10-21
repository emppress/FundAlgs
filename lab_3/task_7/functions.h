#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

#include "status.h"
#include "unidirectional_list.h"

typedef struct SNode
{
    char c;
    struct SNode *next;
} SNode;

typedef struct Stack
{
    SNode *head;
    size_t size;
} Stack;

void print_menu();
status get_liver(Liver *liver);
status stack_init(Stack *stack);
status stack_push(Stack *stack, char c);
status stack_pop(Stack *stack, char *top);
status stack_destroy(Stack *stack);
void free_buf();

#endif