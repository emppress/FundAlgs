#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define CAPACITY 10

typedef enum Status
{
    OK,
    INPUT_ERROR,
    MEMORY_ERROR
} status;

status string_to_int(const char *str, long *result);
void generate_static_array(long array[], long a, long b);
void swap_min_max(long array[]);
status generate_dynamic_array(int **array, int size_array);
void generate_sum_array(int *A, int *B, int *C, int size_array);

#endif