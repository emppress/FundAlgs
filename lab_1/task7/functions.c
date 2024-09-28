#include "functions.h"

status read_word_from_file(FILE *file, char **word, int *len_word)
{
    int capacity = 20;
    *len_word = 0;
    char symbol = fgetc(file);
    while (symbol == ' ' || symbol == '\t' || symbol == '\n')
    {
        if (symbol == EOF)
            return OK;
        symbol = fgetc(file);
    }

    *word = (char *)malloc(capacity * sizeof(char));
    if (*word == NULL)
    {
        return MEMORY_ERROR;
    }

    while (symbol != ' ' && symbol != '\t' && symbol != '\n')
    {
        if (*len_word + 1 == capacity)
        {
            capacity *= 2;
            char *for_resize = (char *)realloc(*word, capacity);
            if (for_resize == NULL)
            {
                free(word);
                return MEMORY_ERROR;
            }
            *word = for_resize;
        }
        (*word)[(*len_word)++] = symbol;
        symbol = fgetc(file);
        if (symbol == EOF)
        {
            (*word)[*len_word] = '\000';
            return OK;
        }
    }
    (*word)[*len_word] = '\000';
    return OK;
}

void word_to_4(char **word, int len_word, int **result)
{
    int symbol_4 = 0;
    for (int i = 0; i < len_word; i++)
    {
        if (isalpha((*word)[i]))
            (*word)[i] = tolower((*word)[i]);

        int shift = 1, symbol = (int)(*word)[i];
        while (symbol)
        {
            symbol_4 += (symbol % 4) * shift;
            shift *= 10;
            symbol /= 4;
        }
        (*result)[i] = symbol_4;

        symbol_4 = 0;
    }
}

void to_lowercase(char **word, int len_word)
{
    for (int i = 0; i < len_word; i++)
    {
        if (isalpha((*word)[i]))
            (*word)[i] = tolower((*word)[i]);
    }
}

void word_to_8(char **word, int len_word, int **result)
{
    int symbol_8 = 0;
    for (int i = 0; i < len_word; i++)
    {
        int shift = 1, symbol = (int)(*word)[i];
        while (symbol)
        {
            symbol_8 += (symbol % 8) * shift;
            shift *= 10;
            symbol /= 8;
        }
        (*result)[i] = symbol_8;
        symbol_8 = 0;
    }
}
