#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

typedef enum Status
{
    OK,
    INPUT_ERROR,
    MEMORY_ERROR
} status;

status string_to_ll(const char *str, long long *result, int base);
void convert_to_xbase(long long num, int base, char *result);

#endif