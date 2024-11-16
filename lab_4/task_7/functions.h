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

typedef enum Status
{
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    MISSING,
    FILE_OPEN_ERROR,
    DIVISION_BY_ZERO_ERROR
} status;

typedef struct String
{
    char *arr;
    size_t capacity;
    size_t len;
} String;

typedef struct MemoryCell
{
    String name;
    int value;
} MemoryCell;

typedef struct Variables
{
    size_t capacity;
    size_t size;
    MemoryCell *array;
} Variables;

status execute_expression(String *expression, Variables *vars);
status init_variables(Variables *vars);
status print_var(const char *var_name, Variables *vars);
status print_all(Variables *vars);
void delete_variables(Variables *vars);
status read_string(FILE *input, String *str);
status delete_string_content(String *string);
status add_char_to_string(String *str, char ch);
status string_init(String *string);
#endif