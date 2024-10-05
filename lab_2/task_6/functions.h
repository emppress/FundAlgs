#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <ctype.h>

typedef enum Status
{
    SUCCESS,
    MEMORY_ERROR,
    INPUT_ERROR,
    FILE_OPEN_ERROR
} status;

int oversscanf(const char *buffer, const char *format, ...);
int overfprintf(FILE *file, const char *format, ...);
int overfscanf(FILE *stream, const char *format, ...);

#endif