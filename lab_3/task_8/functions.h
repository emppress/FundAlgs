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
    ZERO_ARGS
} status;

typedef struct String
{
    char *arr;
    size_t len, capacity;
} String;
typedef struct Term
{
    int coef;
    int degree;
    struct Term *next;
} Term;

typedef struct Polynom
{
    size_t size;
    Term *head;
} Polynom;

void free_buf();
status init_polynom(Polynom *polynom);
status insert_term(Polynom *polynom, int degree, int coef);
void destroy_polynom(Polynom *polynom);
status create_polynom(Polynom *polynom, const char *expression);
status add(Polynom *polynom_1, Polynom *polynom_2);
status mult(Polynom *polynom_1, Polynom *polynom_2, Polynom *res);
status eval(Polynom *polynom, double x, double *result);
status diff(Polynom *polynom, Polynom *res);
status cmps(Polynom *polynom_1, Polynom *polynom_2, Polynom *result);
status div_mod(Polynom *dividend, Polynom *divider, Polynom *res);
status sub(Polynom *polynom_1, Polynom *polynom_2);
status print_polynom(Polynom *polynom);

status get_expression(Polynom *polynom_1, Polynom *polynom_2, FILE *input, int *count);

#endif