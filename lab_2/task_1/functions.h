#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    FILE_OPEN_ERROR
} status;

status validate_input(int argc, char *argv[]);
int my_strlen(char const *str);
status my_strrev(const char *src, char **dest);
status up_odd_str(const char *src, char **dest);
status transform_str(const char *src, char **dest);
status randomly_concatenate(const char **strings, int count_stings, unsigned long seed, char **dest);
void concatenate(const char *src, char *dest);
status string_to_uint(const char *str, unsigned long *result);

#endif