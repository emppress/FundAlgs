#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>

typedef enum Status
{
    SUCCESS,
    MEMORY_ERROR,
    INPUT_ERROR,
    FILE_OPEN_ERROR
} status;

int oversprintf(char *str, const char *format, ...);
int overfprintf(FILE *file, const char *format, ...);

#endif