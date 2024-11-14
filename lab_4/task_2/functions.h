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

#define START_HASH_SIZE 128
#define FREE_STR(str) \
    if (str)          \
    {                 \
        free(str);    \
        str = NULL;   \
    }

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    MISSING,
    FILE_OPEN_ERROR
} status;

typedef struct Array
{
    char name;
    int *data;
    size_t size, capacity;
} Array;

void init_array(Array *arr, char name);
status load_arr(Array *arr, FILE *input);
status rand_arr(Array *arr, int count, int lb, int rb);
void delete_array(Array *arr);
status concat_arr(Array *arr_1, Array *arr_2);
status remove_arr(Array *arr, size_t lb, size_t count);
status copy_arr(Array *dest, Array *sourse, size_t lb, size_t rb);
int compare_a(const void *a, const void *b);
int compare_d(const void *a, const void *b);
int compare_s(const void *a, const void *b);
status print_one_element(Array *arr, size_t idx);
status print_all(Array *arr);
status print_section(Array *arr, size_t lb, size_t rb);
status print_stats(Array *arr);
status read_command(char **command, FILE *input);
void delete_all(Array *arr);
status save_arr(Array *arr, FILE *output);
status string_to_int(const char *str, long *result);
status string_to_size_t(const char *str, size_t *result);

#endif