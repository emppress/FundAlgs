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

typedef struct Vector
{
    double *data;
    size_t n;
} vector;

typedef double (*norm_t)(vector *, void *);

double spectr_norm(vector *vec, void *args);
double inf_norm(vector *vec, void *args);
double p_norm(vector *vec, void *args);
status search_longest_vectors(vector **res, size_t *count_res, size_t vectors_cnt, size_t count_norms, ...);

#endif