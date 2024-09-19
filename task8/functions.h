#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>

#define CAPACITY 70

typedef enum Status
{
    OK,
    INPUT_ERROR,
    MEMORY_ERROR,
    FILE_OPEN_ERROR
} status;

status read_num_from_file(FILE *file, char *word, int *len_word, int *base);
status convert_xbase_to_ll(const char *const word, int base, long long *result);

#endif