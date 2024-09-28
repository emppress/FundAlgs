#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

typedef enum Status
{
    OK,
    INPUT_ERROR,
    MEMORY_ERROR,
    FILE_OPEN_ERROR
} status;

void count_letters(FILE *in, FILE *out);
void exclude_nums(FILE *in, FILE *out);
void count_symbols(FILE *in, FILE *out);
void replace_symbols(FILE *in, FILE *out);

#endif