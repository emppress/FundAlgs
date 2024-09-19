#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

typedef enum Status
{
    OK,
    INPUT_ERROR,
    MEMORY_ERROR
} status;

status string_to_int(const char *str, long *result);
status natural_nums(const long x, int **result, int *size);
int is_prime(const long x);
status sum(const long x, long long *result);
status fact(const long x, long long *result);
status degree_table(const long x);
status split_16(long x, char **result, int *size);

#endif