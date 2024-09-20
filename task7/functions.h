#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "stdio.h"
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

status read_word_from_file(FILE *file, char **word, int *len_word);
void word_to_4(char **word, int len_word, int **result);
void word_to_8(char **word, int len_word, int **result);
void to_lowercase(char **word, int len_word);

#endif