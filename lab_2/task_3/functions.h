#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    FILE_OPEN_ERROR
} status;

typedef struct Found
{
    char *file_name;
    int n_line;
    int n_char;
    struct Found *next;
} found;

status search_str_in_files(char *substring, int count_files, found **result, ...);
#endif