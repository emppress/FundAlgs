#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "stdio.h"
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} status;

status sum_any_base_all_numbers(char **result, int base, int count_numbers, ...);
status validate_base(char *number, int base, int len);
void str_reverse(char *str, int len);
void sum_any_base_two_numbers(char *number1_res, char *number2, int base, int *len_res, int len_number);
#endif