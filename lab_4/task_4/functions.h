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

#define COUNT_VECTORS 26

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

typedef struct Vector
{
    char name;
    long value;
    char init;
} Vector;

status read_string(FILE *input, String *str);
status delete_string_content(String *string);
status add_char_to_string(String *str, char ch);
status string_init(String *string);
status init_vectors_arr(Vector *v_arr);
void write_message(const char *msg, FILE *out, ...);
status read_vector(Vector *vector, const char *value, long base);
status read_word(String *str);
status string_to_long(const char *str, long *result, int base);
status convert_to_xbase(long num, long base, char *result);
status calculate_operation(Vector *a, Vector *b, const char *op, Vector *result);
status convert_to_bitstr(long num, char *result);
status validate_file(char **argv);
#endif